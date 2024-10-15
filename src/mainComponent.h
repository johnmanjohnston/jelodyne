#pragma once

#include "clip.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_audio_utils/juce_audio_utils.h"
#include "juce_graphics/juce_graphics.h"
#include "piano.h"
#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
                      public juce::MidiInputCallback,
                      private juce::Timer {
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

    jelodyne::clip clip;

    void handleIncomingMidiMessage(juce::MidiInput *source,
                                   const juce::MidiMessage &message) override;

    // pitch detection
    static constexpr auto fftOrder = 13;
    static constexpr auto fftSize = 1 << fftOrder;
    juce::dsp::FFT forwardFFT;
    juce::Image spectrogramImage;

    std::array<float, fftSize> fifo;        // [4]
    std::array<float, fftSize * 2> fftData; // [5]
    int fifoIndex = 0;                      // [6]
    bool nextFFTBlockReady = false;         // [7]

    void pushNextSampleIntoFifo(float sample);
    void drawNextLineOfSpectrogram();

    juce::String frequencyToNote(float frequency);

    void timerCallback() override;

    double sample_rate;

    //==============================================================================
    // Your private member variables go here...

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
