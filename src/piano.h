#pragma once
#include "juce_audio_basics/juce_audio_basics.h"
#include <JuceHeader.h>

// when user is correcting their pitch, it's useful to have a piano as well to
// use as reference--this class is that piano
namespace jelodyne::piano {
class pianoRoll : public juce::MidiKeyboardComponent {
  public:
    pianoRoll(MidiKeyboardState &state, Orientation orientation);

  protected:
    virtual void drawWhiteNote(int midiNoteNumber, Graphics &g,
                               Rectangle<float> area, bool isDown, bool isOver,
                               Colour lineColour, Colour textColour) override;

    virtual void drawBlackNote(int midiNoteNumber, Graphics &g,
                               Rectangle<float> area, bool isDown, bool isOver,
                               Colour noteFillColour) override;
};

class pianoSynth : public juce::Synthesiser {
  public:
    void init(juce::String sample_path);

  private:
    juce::String piano_sample_path;
    juce::AudioFormatManager afm;
    std::unique_ptr<juce::AudioFormatReader> reader;
};
} // namespace jelodyne::piano
