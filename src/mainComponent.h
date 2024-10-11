#pragma once

#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "piano.h"
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::MidiInputCallback,
                      public juce::MidiKeyboardState::Listener {
  public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(
        const juce::AudioSourceChannelInfo &bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint(juce::Graphics &g) override;
    void resized() override;

    jelodyne::piano::piano_roll piano_roll;
    juce::MidiKeyboardState kb_state;
    juce::MidiBuffer midi_buf;

    jelodyne::piano::piano_synth synth;

    juce::AudioDeviceManager device_manager;
    double start_time;

    juce::Random random;

    void handleIncomingMidiMessage(juce::MidiInput *source,
                                   const juce::MidiMessage &message) override;

    void handleNoteOn(juce::MidiKeyboardState *src, int a, int b,
                      float x) override;
    void handleNoteOff(juce::MidiKeyboardState *src, int a, int b,
                       float x) override;
    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
