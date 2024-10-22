#include "mainComponent.h"
#include "note.h"
#include "piano.h"
#include "utility"
#include "utility.h"
#include <cmath>

//==============================================================================
MainComponent::MainComponent()
    : piano_roll(kb_state,
                 juce::KeyboardComponentBase::verticalKeyboardFacingRight),
      start_time(juce::Time::getMillisecondCounterHiRes() * 0.001),
      forwardFFT(fftOrder), spectrogramImage(juce::Image::RGB, 512, 512, true) {
    // Make sure you set the size of the component after
    // you add any child components.
    setSize(800, 600);

    // Some platforms require permissions to open input channels so request that
    // here
    if (juce::RuntimePermissions::isRequired(
            juce::RuntimePermissions::recordAudio) &&
        !juce::RuntimePermissions::isGranted(
            juce::RuntimePermissions::recordAudio)) {
        juce::RuntimePermissions::request(
            juce::RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    } else {
        // Specify the number of input and output channels that we want to open
        setAudioChannels(2, 2);
    }

    addAndMakeVisible(piano_roll);

    // TODO: move this to prepareToPlay() instead
    auto midi_inputs = juce::MidiInput::getAvailableDevices();
    juce::String midiName = "";
    for (auto i : midi_inputs) {
        //   if (device_manager.isMidiInputDeviceEnabled(i.identifier)) {
        //     device_manager.setMidiInputDeviceEnabled(i.identifier, true);
        //}
        //
        midiName = i.name;
        device_manager.setMidiInputDeviceEnabled(i.identifier, true);
        device_manager.addMidiInputDeviceCallback(i.identifier, this);
    }

    DBG("activating device " << midiName);
    startTimerHz(60);

    load_file("/home/johnston/Downloads/jelodyne-testing.wav");
}

void MainComponent::timerCallback() {
    // timer callback
    if (nextFFTBlockReady) {
        drawNextLineOfSpectrogram();
        nextFFTBlockReady = false;
        repaint();
    }
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    stopTimer();
}

void MainComponent::handleIncomingMidiMessage(
    juce::MidiInput *source, const juce::MidiMessage &message) {
    kb_state.processNextMidiEvent(message);

    midi_buf.addEvent(message, 0);

    const juce::MessageManagerLock
        mm_lock; // we're calling a function running on the GUI thread, so we
                 // need MessageManagerLock for thread safety
    piano_roll.repaint();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected,
                                  double sampleRate) {
    this->sample_rate = sampleRate;

    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.init("/home/johnston/Downloads/jelodyne-testing.wav");
    kb_state.reset();

    clip.path = "/home/johnston/Downloads/acapella.wav";
}

void MainComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo &bufferToFill) {
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    //  Right now we are not producing any data, in which case we need to clear
    // the buffer (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

    kb_state.processNextMidiBuffer(midi_buf, bufferToFill.startSample,
                                   bufferToFill.numSamples, true);

    synth.renderNextBlock(*bufferToFill.buffer, midi_buf,
                          bufferToFill.startSample, bufferToFill.numSamples);

    midi_buf.clear();

    if (bufferToFill.buffer->getNumChannels() > 0) {
        auto *channelData =
            bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i)
            pushNextSampleIntoFifo(channelData[i]);
    }

    if (analyze_file) {
        if (file_buffer.getNumChannels() > 0) {
            auto *channelData = file_buffer.getReadPointer(0);
            int numSamples = file_buffer.getNumSamples();
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
                        (float)maxIndex * this->sample_rate / fftSize;

                    // TODO: optimize the insertion of notes into the notes
                    // vector--get rid of the conversion from frequency to
                    // string to note number and instead, just directly convert
                    // to note number

                    // only add to notes if notes is between C2 and C5
                    if (frequency > 65.f && frequency < 520) {
                        jelodyne::note note;
                        note.note_number = jelodyne::note_name_to_number(
                            frequencyToNote(frequency), 3);

                        if (note.note_number != -1) {
                            note.start_sample = i;
                            note.original_frequency = frequency;

                            this->file_notes.push_back(note);
                        }
                    }
                }
            }
            analyze_file = false;
        }
        DBG("analysis for file done.");

        // set end samples for all notes
        for (std::vector<jelodyne::note>::size_type i = 0;
             i != file_notes.size(); i++) {

            if (i + 1 != file_notes.size()) {
                file_notes[i].end_sample = file_notes[i + 1].start_sample;
            }
        }

        jelodyne::consolidate_duplicate_notes(this->file_notes);
        jelodyne::consolidate_duplicate_notes(this->file_notes);
        jelodyne::consolidate_duplicate_notes(this->file_notes);
        jelodyne::consolidate_duplicate_notes(this->file_notes);

        /*
        for (auto n : file_notes) {
            DBG("note " << juce::MidiMessage::getMidiNoteName(n.note_number,
                                                              true, true, 3)
                        << " starts at " << n.start_sample << " and ends at "
                        << n.end_sample << " and has an original frequency of "
                        << n.original_frequency);
        }*/
        jelodyne::remove_pitch_artifacts(this->file_notes, this->fftSize);

        jelodyne::consolidate_duplicate_notes(this->file_notes);
        jelodyne::consolidate_duplicate_notes(this->file_notes);
        jelodyne::consolidate_duplicate_notes(this->file_notes);

        for (std::vector<jelodyne::note>::size_type i = 0;
             i != file_notes.size(); i++) {

            DBG("note is " << juce::MidiMessage::getMidiNoteName(
                                  file_notes[i].note_number, true, true, 3)
                           << " and number lasts "
                           << (file_notes[i].end_sample -
                               file_notes[i].start_sample)
                           << " samples");
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

// TODO: we don't need this function--we're not rendering any spectrograms,
// so get rid of it

void MainComponent::drawNextLineOfSpectrogram() {
    return;
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // move image leftwards
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge,
                                      imageHeight); // [1]

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data()); // [2]

    int maxIndex = 0;
    float maxValue = 0.0f;
    for (int i = 0; i < fftSize / 2; ++i) {
        if (fftData[(size_t)i] > maxValue) {
            maxValue = fftData[(size_t)i];
            maxIndex = i;
        }
    }

    float frequency = (float)maxIndex * this->sample_rate / fftSize;

    if (frequency != 0.f)
        // DBG("frequency " << frequency);
        DBG("note is " << frequencyToNote(frequency));

    // specrogram rendering code
    // find the range of values produced, so we can scale our rendering to
    // show up the detail clearly
    auto maxLevel = juce::FloatVectorOperations::findMinAndMax(
        fftData.data(), fftSize / 2); // [3]

    juce::Image::BitmapData bitmap{spectrogramImage,
                                   rightHandEdge,
                                   0,
                                   1,
                                   imageHeight,
                                   juce::Image::BitmapData::writeOnly}; // [4]

    for (auto y = 1; y < imageHeight; ++y) // [5]
    {
        auto skewedProportionY =
            1.0f - std::exp(std::log((float)y / (float)imageHeight) * 0.2f);
        auto fftDataIndex = (size_t)juce::jlimit(
            0, fftSize / 2, (int)(skewedProportionY * fftSize / 2));
        auto level =
            juce::jmap(fftData[fftDataIndex], 0.0f,
                       juce::jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

        juce::Colour col = juce::Colour::fromHSV(0, 1.f, level, 1.f);

        bitmap.setPixelColour(0, y, col);
    }
}

void MainComponent::load_file(juce::String path) {
    DBG("load_file() called");

    if (afm.findFormatForFileExtension("wav") == nullptr)
        afm.registerBasicFormats();

    std::unique_ptr<juce::AudioFormatReader> reader(afm.createReaderFor(path));

    if (reader.get() != nullptr) {
        auto duration = reader->lengthInSamples / reader->sampleRate;
        file_buffer.setSize((int)reader->numChannels,
                            (int)reader->lengthInSamples);

        reader->read(&file_buffer, 0, (int)reader->lengthInSamples, 0, true,
                     true);

        analyze_file = true;
    } else {
        DBG("READER FOR " << path << " DOES NOT EXIST");
    }
}

juce::String MainComponent::frequencyToNote(float input) {
    // yoink implementation used in https://www.phys.unsw.edu.au/music/note/
    // (see page source to find orginal implementation)

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
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background
    // with a solid colour)
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.drawText("this text is being drawn from code written in neovim", 10, 10,
               400, 10, juce::Justification::left, false);

    g.drawImage(spectrogramImage, 20, 20, 400, 400, 0, 0,
                spectrogramImage.getWidth(), spectrogramImage.getHeight(),
                false);
}

void MainComponent::resized() { piano_roll.setBounds(0, 0, 64, 600); }
