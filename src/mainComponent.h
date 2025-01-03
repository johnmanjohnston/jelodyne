#pragma once

#include "clip.h"
#include "jlistener/jlistenerbroadcaster.h"
#include "lookandfeel.h"
#include "note.h"
#include "noteInfoComponent.h"
#include "piano.h"

#include "controlbar.h"
#include "noteComponent.h"
#include "playhead.h"

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

// TODO: change privacy of class members
class MainComponent : public juce::AudioAppComponent,
                      public juce::MidiInputCallback,
                      public JListener {
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

    jelodyne::JelodyneLookAndFeel jLookAndFeel;

    // void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void JListenerCallback(void *data, void *metadata,
                           JBroadcaster *source) override;

    int EDITOR_HEIGHT = 720;
    int EDITOR_WIDTH = 1280;

    int WINDOW_HEIGHT = EDITOR_HEIGHT + 30;
    int WINDOW_WIDTH = EDITOR_WIDTH;

    int midiKeyboardWidth = 64;
    jelodyne::piano::pianoRoll pianoRoll;
    juce::MidiKeyboardState kbState;
    juce::MidiBuffer midiBuffer;

    jelodyne::playhead playheadComponent;

    jelodyne::piano::pianoSynth synth;

    juce::AudioDeviceManager deviceManager;
    double startTime;

    juce::Random random;

    jelodyne::clip clip;

    void handleIncomingMidiMessage(juce::MidiInput *source,
                                   const juce::MidiMessage &message) override;

    // pitch detection
    static constexpr auto fftOrder = 13;
    static constexpr auto fftSize = 2 << fftOrder;
    juce::dsp::FFT forwardFFT;

    std::array<float, fftSize> fifo = {0};
    std::array<float, fftSize * 2> fftData = {0};
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;

    void pushNextSampleIntoFifo(float sample);

    juce::String frequencyToNote(float frequency);

    double mSampleRate;

    juce::AudioFormatManager afm;
    juce::AudioBuffer<float> fileBuffer;
    // juce::AudioBuffer<float> currentNoteBuffer;
    void loadFile(juce::String path);
    bool analyzeFile = false;
    bool addedNoteComponents = false;

    std::vector<jelodyne::note> fileNotes;
    std::vector<std::unique_ptr<jelodyne::NoteComponent>> noteComponents;
    bool shouldPlayLoopingNote();
    int position = 0;
    jelodyne::note *currentLoopingNote = nullptr;
    jelodyne::NoteInfoComponent *currentNoteInfoComponent = nullptr;

    jelodyne::ControlBar controlBar;

    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
