#include "mainComponent.h"
#include "piano.h"

//==============================================================================
MainComponent::MainComponent()
    : piano_roll(kb_state,
                 juce::KeyboardComponentBase::verticalKeyboardFacingRight),
      start_time(juce::Time::getMillisecondCounterHiRes() * 0.001) {
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
}

MainComponent::~MainComponent() {
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
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

    // Right now we are not producing any data, in which case we need to clear
    // the buffer (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();

    kb_state.processNextMidiBuffer(midi_buf, bufferToFill.startSample,
                                   bufferToFill.numSamples, true);

    synth.renderNextBlock(*bufferToFill.buffer, midi_buf,
                          bufferToFill.startSample, bufferToFill.numSamples);

    midi_buf.clear();
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
}

void MainComponent::resized() {
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    piano_roll.setBounds(0, 0, 64, 600);
}
