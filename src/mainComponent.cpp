#include "mainComponent.h"
#include "piano.h"

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
    synth.setCurrentPlaybackSampleRate(sampleRate);
    synth.init("/home/johnston/Downloads/piano.wav");
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

void MainComponent::drawNextLineOfSpectrogram() {

    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // move image leftwards
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge,
                                      imageHeight); // [1]

    // then render our FFT data..
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data()); // [2]

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

void MainComponent::releaseResources() {
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint(juce::Graphics &g) {
    // (Our component is opaque, so we must completely fill the background with
    // a solid colour)
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.drawText("this text is being drawn from code written in neovim", 10, 10,
               400, 10, juce::Justification::left, false);

    g.drawImage(spectrogramImage, 20, 20, 400, 400, 0, 0,
                spectrogramImage.getWidth(), spectrogramImage.getHeight(),
                false);
}

void MainComponent::resized() {
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    piano_roll.setBounds(0, 0, 64, 600);
}
