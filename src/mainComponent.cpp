#include "mainComponent.h"
#include "common.h"
#include "note.h"
#include "noteComponent.h"
#include "piano.h"
#include "scale.h"
#include "utility.h"
#include <cmath>
#include <vector>

//==============================================================================
MainComponent::MainComponent()
    : // juce::ChangeListener(),
      pianoRoll(kbState,
                juce::KeyboardComponentBase::verticalKeyboardFacingRight),
      playheadComponent(), controlBar(),
      startTime(juce::Time::getMillisecondCounterHiRes() * 0.001),
      forwardFFT(fftOrder) {
    setSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    if (juce::RuntimePermissions::isRequired(
            juce::RuntimePermissions::recordAudio) &&
        !juce::RuntimePermissions::isGranted(
            juce::RuntimePermissions::recordAudio)) {
        juce::RuntimePermissions::request(
            juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    } else {
        setAudioChannels(2, 2);
    }

    // pianoRoll.pianoScale.updateScale(KEY_Db, MINOR, PENTATONIC);
    // pianoRoll.pianoScale.updateScale(KEY_Db, MINOR, PENTATONIC);

    pianoRoll.pianoScale.updateScale(KEY_NO_KEY, NO_TONALITY, NO_SCALE);

    addAndMakeVisible(pianoRoll);
    pianoRoll.setKeyWidth(32.7f);
    pianoRoll.setAvailableRange(12 * (2 + 2), 12 * (5 + 2)); // C2 to C5

    addAndMakeVisible(playheadComponent);

    addAndMakeVisible(controlBar);
    controlBar.setListener(this);

    // TODO: move this to prepareToPlay() instead
    auto midi_inputs = juce::MidiInput::getAvailableDevices();
    juce::String midiName = "";
    for (auto i : midi_inputs) {
        midiName = i.name;
        deviceManager.setMidiInputDeviceEnabled(i.identifier, true);
        deviceManager.addMidiInputDeviceCallback(i.identifier, this);
    }

    DBG("activating device " << midiName);

    loadFile("/home/johnston/Downloads/jelodyne-testing.wav");
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::handleIncomingMidiMessage(
    juce::MidiInput *source, const juce::MidiMessage &message) {
    kbState.processNextMidiEvent(message);

    midiBuffer.addEvent(message, 0);

    const juce::MessageManagerLock
        mm_lock; // we're calling a function running on the GUI thread, so we
                 // need MessageManagerLock for thread safety
    pianoRoll.repaint();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {
    this->mSampleRate = sampleRate;

    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.init("/home/johnston/Downloads/jelodyne-testing.wav");
    kbState.reset();

    clip.path = "/home/johnston/Downloads/acapella.wav";
}

bool MainComponent::shouldPlayLoopingNote() {
    if (this->currentLoopingNote == nullptr)
        return false;

    if (this->currentLoopingNote->buffer.getNumSamples() == 0)
        return false;

    return true;
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {
    bufferToFill.clearActiveBufferRegion();

    kbState.processNextMidiBuffer(midiBuffer, bufferToFill.startSample,
                                  bufferToFill.numSamples, true);

    synth.renderNextBlock(*bufferToFill.buffer, midiBuffer,
                          bufferToFill.startSample, bufferToFill.numSamples);

    midiBuffer.clear();

    if (bufferToFill.buffer->getNumChannels() > 0) {
        auto *channelData =
            bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
            pushNextSampleIntoFifo(channelData[i]);
    }

    if (analyzeFile) {
        if (fileBuffer.getNumChannels() > 0) {
            auto *channelData = fileBuffer.getReadPointer(0);
            int numSamples = fileBuffer.getNumSamples();
            for (auto i = 0; i < numSamples; i++) {
                pushNextSampleIntoFifo(channelData[i]);
                if (nextFFTBlockReady) {
                    // drawNextLineOfSpectrogram();
                    nextFFTBlockReady = false;

                    forwardFFT.performFrequencyOnlyForwardTransform(
                        fftData.data()); // [2]

                    int maxIndex = 0;
                    float maxValue = 0.0f;
                    for (int j = 0; j < fftSize / 2; ++j) {
                        if (fftData[(size_t)j] > maxValue) {
                            maxValue = fftData[(size_t)j];
                            maxIndex = j;
                        }
                    }

                    float frequency =
                        (float)maxIndex * this->mSampleRate / fftSize;

                    // TODO: optimize the insertion of notes into the notes
                    // vector--get rid of the conversion from frequency to
                    // string to note number and instead, just directly convert
                    // to note number

                    // only add to notes if notes is between C2 and C5
                    // also, we aren't setting end sample here--we do it later
                    if (frequency > 65.f && frequency < 520) {
                        jelodyne::note note;
                        note.noteNumber = jelodyne::noteNameToNumber(
                            frequencyToNote(frequency), 3);

                        if (note.noteNumber != -1) {
                            note.startSample = i;
                            note.originalFrequency = frequency;

                            this->fileNotes.push_back(note);
                        }
                    }
                }
            }
            analyzeFile = false;
        }
        DBG("analysis for file done.");

        // set end samples for all notes
        for (std::vector<jelodyne::note>::size_type i = 0;
             i != fileNotes.size(); i++) {

            if (i + 1 != fileNotes.size()) {
                fileNotes[i].endSample = fileNotes[i + 1].startSample;
            }
        }

        // TODO: cleanup
        jelodyne::consolidateDuplicateNotes(this->fileNotes);
        jelodyne::consolidateDuplicateNotes(this->fileNotes);
        jelodyne::consolidateDuplicateNotes(this->fileNotes);
        jelodyne::consolidateDuplicateNotes(this->fileNotes);

        /*
        for (auto n : file_notes) {
            DBG("note " << juce::MidiMessage::getMidiNoteName(n.note_number,
                                                              true, true, 3)
                        << " starts at " << n.start_sample << " and ends at "
                        << n.end_sample << " and has an original frequency of "
                        << n.original_frequency);
        }*/
        jelodyne::removePitchArtifacts(this->fileNotes, this->fftSize);

        jelodyne::consolidateDuplicateNotes(this->fileNotes);
        jelodyne::consolidateDuplicateNotes(this->fileNotes);
        jelodyne::consolidateDuplicateNotes(this->fileNotes);

        // now that we've gotten rid of duplicates/artifacts, copy the section
        // of the buffer which coressponds to the note
        int numChannels = fileBuffer.getNumChannels();
        int totalSamples = fileBuffer.getNumSamples();
        for (auto &note : fileNotes) {
            // copy only the data from current note from file_buffer into
            // note.buffer
            int numSamples = note.endSample - note.startSample;
            int startSample = note.startSample;

            // don't copy buffer if sample numbers are invalid
            if (startSample < 0 || numSamples <= 0 ||
                startSample + numSamples > totalSamples) {
                return;
            }

            note.buffer.setSize(numChannels, numSamples);

            for (int channel = 0; channel < numChannels; ++channel) {
                note.buffer.copyFrom(channel, 0, fileBuffer, channel,
                                     startSample, numSamples);
            }
        }

        for (std::vector<jelodyne::note>::size_type i = 0;
             i != fileNotes.size(); i++) {

            DBG("note is "
                << juce::MidiMessage::getMidiNoteName(fileNotes[i].noteNumber,
                                                      true, true, 3)
                << " and number lasts "
                << (fileNotes[i].endSample - fileNotes[i].startSample)
                << " which is "
                << (fileNotes[i].endSample - fileNotes[i].startSample) / 41000.0
                << " seconds" << " samples");
        }
    }

    if (shouldPlayLoopingNote() == true) {
        // loop the current note so that the user knows what pitch
        // they're singing

        auto numInputChannels = currentLoopingNote->buffer.getNumChannels();
        auto numOutputChannels = currentLoopingNote->buffer.getNumChannels();

        auto outputSamplesRemaining = bufferToFill.numSamples; // [8]
        auto outputSamplesOffset = bufferToFill.startSample;

        while (outputSamplesRemaining > 0) {
            auto bufferSamplesRemaining =
                currentLoopingNote->buffer.getNumSamples() - position; // [10]
            auto samplesThisTime = juce::jmin(outputSamplesRemaining,
                                              bufferSamplesRemaining); // [11]

            for (auto channel = 0; channel < numOutputChannels; ++channel) {
                bufferToFill.buffer->copyFrom(
                    channel,                    // [12]
                    outputSamplesOffset,        //  [12.1]
                    currentLoopingNote->buffer, //  [12.2]
                    channel % numInputChannels, //  [12.3]
                    position,                   //  [12.4]
                    samplesThisTime);           //  [12.5]
            }

            outputSamplesRemaining -= samplesThisTime; // [13]
            outputSamplesOffset += samplesThisTime;    // [14]
            position += samplesThisTime;               // [15]

            if (position == currentLoopingNote->buffer.getNumSamples())
                position = 0; // [16]
        }
    }
}

void MainComponent::pushNextSampleIntoFifo(float sample) {
    // if the fifo contains enough data, set a flag to say
    // that the next line should now be rendered..
    if (fifoIndex == fftSize) // [8]
    {
        if (!nextFFTBlockReady) // [9]
        {
            std::fill(fftData.begin(), fftData.end(), 0.0f);
            std::copy(fifo.begin(), fifo.end(), fftData.begin());
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[(size_t)fifoIndex++] = sample; // [9]
}

void MainComponent::loadFile(juce::String path) {
    DBG("load_file() called");

    if (afm.findFormatForFileExtension("wav") == nullptr)
        afm.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(afm.createReaderFor(path));

    if (reader.get() != nullptr) {
        auto duration = reader->lengthInSamples / reader->sampleRate;
        fileBuffer.setSize((int)reader->numChannels,
                           (int)reader->lengthInSamples);

        reader->read(&fileBuffer, 0, (int)reader->lengthInSamples, 0, true,
                     true);
        analyzeFile = true;
    } else {
        DBG("READER FOR " << path << " DOES NOT EXIST");
    }
}

juce::String MainComponent::frequencyToNote(float input) {
    // yoink implementation used in
    // https://www.phys.unsw.edu.au/music/note/ (see page source to find
    // orginal implementation)

    // TODO: optmize this function--notes[] has many elements which
    // aren't even humanly possible to sing, so get rid of them
    double a4 = 440.0;
    int a4_index = 57;

    std::string notes[] = {
        "C0",  "C#0", "D0",  "D#0", "E0",  "F0",  "F#0", "G0",  "G#0", "A0",
        "A#0", "B0",  "C1",  "C#1", "D1",  "D#1", "E1",  "F1",  "F#1", "G1",
        "G#1", "A1",  "A#1", "B1",  "C2",  "C#2", "D2",  "D#2", "E2",  "F2",
        "F#2", "G2",  "G#2", "A2",  "A#2", "B2",  "C3",  "C#3", "D3",  "D#3",
        "E3",  "F3",  "F#3", "G3",  "G#3", "A3",  "A#3", "B3",  "C4",  "C#4",
        "D4",  "D#4", "E4",  "F4",  "F#4", "G4",  "G#4", "A4",  "A#4", "B4",
        "C5",  "C#5", "D5",  "D#5", "E5",  "F5",  "F#5", "G5",  "G#5", "A5",
        "A#5", "B5",  "C6",  "C#6", "D6",  "D#6", "E6",  "F6",  "F#6", "G6",
        "G#6", "A6",  "A#6", "B6",  "C7",  "C#7", "D7",  "D#7", "E7",  "F7",
        "F#7", "G7",  "G#7", "A7",  "A#7", "B7",  "C8",  "C#8", "D8",  "D#8",
        "E8",  "F8",  "F#8", "G8",  "G#8", "A8",  "A#8", "B8",  "C9",  "C#9",
        "D9",  "D#9", "E9",  "F9",  "F#9", "G9",  "G#9", "A9",  "A#9", "B9"};

    auto r = std::pow(2.0, 1.0 / 12.0);
    auto cent = std::pow(2.0, 1.0 / 1200.0);
    auto r_index = 0;
    auto cent_index = 0;

    auto frequency = a4;

    if (input >= frequency) {
        while (input >= r * frequency) {
            frequency = r * frequency;
            r_index++;
        }

        while (input > cent * frequency) {
            frequency = cent * frequency;
            cent_index++;
        }

        if ((cent * frequency - input) < (input - frequency)) {
            cent_index++;
        }

        if (cent_index > 50) {
            r_index++;
            cent_index = 100 - cent_index;
        }
    }

    else {
        while (input <= frequency / r) {
            frequency = frequency / r;
            r_index--;
        }

        while (input < frequency / cent) {
            frequency = frequency / cent;
            cent_index++;
        }

        if ((input - frequency / cent) < (frequency - input)) {
            cent_index++;
        }

        if (cent_index >= 50) {
            r_index--;
            cent_index = 100 - cent_index;
        }
    }

    int retval_index = a4_index + r_index;
    if (retval_index > 110)
        return "invalid note";
    else
        return notes[retval_index];
}

void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is
    // being restarted due to a setting change.

    // For more details, see the help for
    // AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour(25, 25, 25));

    const int startNote = pianoRoll.getRangeStart();
    const int endNote = pianoRoll.getRangeEnd();
    int cellHeight = 19;
    int cellWidth = 22;
    g.setFont(16.f);

    for (int i = 0; i <= endNote - startNote; ++i) {
        int noteNumber = endNote - i;

        bool isBlack = juce::MidiMessage::isMidiNoteBlack(noteNumber);
        juce::Colour noteColour =
            isBlack ? juce::Colour(23, 23, 23) : juce::Colour(45, 45, 45);
        g.setColour(noteColour);

        auto yVal = jelodyne::getYCoordinateForNote(noteNumber, endNote);

        juce::Rectangle<int> drawArea;
        drawArea.setBounds(midiKeyboardWidth, yVal, cellWidth, cellHeight);

        for (int x = 0; x < (getWidth() - midiKeyboardWidth) / cellWidth; ++x) {
            drawArea.setX(midiKeyboardWidth + (x * cellWidth));

            // fill note
            g.setColour(noteColour);
            g.fillRect(drawArea);

            // draw outline to make a grid
            g.setColour(juce::Colour(121, 121, 121).withAlpha(.2f));
            g.drawRect(drawArea);
        }
    }

    if (analyzeFile == false && addedNoteComponents == false) {
        this->noteComponents.resize(fileNotes.size());

        // TODO: find a way to actually position these properly later on
        for (auto n : fileNotes) {
            // DBG("creating NoteComponent instances...");
            auto rect = juce::Rectangle<int>(
                midiKeyboardWidth + (cellWidth * (n.startSample / 18000)),
                jelodyne::getYCoordinateForNote(n.noteNumber, endNote),
                cellWidth, cellHeight);

            auto x = std::make_unique<jelodyne::NoteComponent>();
            x->noteData = n;
            x->orginalNoteData = n;
            x->setListener(this);

            x->setBounds(rect);
            addAndMakeVisible(*x);

            this->noteComponents.push_back(std::move(x));
        }

        addedNoteComponents = true;
    }
}

void MainComponent::resized() {
    pianoRoll.setBounds(0, 0, this->midiKeyboardWidth, EDITOR_HEIGHT);
    playheadComponent.setBounds(WINDOW_WIDTH / 2, 0, 20, WINDOW_HEIGHT);

    /*
    keySelectorBox.setBounds(1, WINDOW_HEIGHT - 20, 100, 20);
    tonalitySelectorBox.setBounds(120, WINDOW_HEIGHT - 20, 100, 20);
    scaleSelectorBox.setBounds(240, WINDOW_HEIGHT - 20, 100, 20);
    */

    int controlBarHeight = 35;
    controlBar.setBounds(0, getBounds().getHeight() - controlBarHeight,
                         WINDOW_WIDTH, controlBarHeight);
}

void MainComponent::JListenerCallback(void *data, void *metadata,
                                      JBroadcaster *source) {
    int typecode = *((int *)(&metadata));

    if (typecode == TYPECODE_NOTE) {
        currentLoopingNote = (jelodyne::note *)data;
    }

    if (typecode == TYPECODE_CLEAR_NOTE) {
        position = 0;
        currentLoopingNote = nullptr;
    }

    if (typecode == TYPECODE_UPDATED_SCALE_DATA) {
        pianoRoll.pianoScale = *((jelodyne::scale *)data);
        pianoRoll.repaint();
    }

    /*
        DBG("start sampmple is " << this->currentNoteStartSample
                                 << " and end sample is "
                                 << this->currentNoteEndSample);
    */
}
