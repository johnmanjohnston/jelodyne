#pragma once
#include "juce_audio_basics/juce_audio_basics.h"
#include <JuceHeader.h>

// when user is correcting their pitch, it's useful to have a piano as well to
// use as reference--this class is that piano
namespace jelodyne::piano {
class piano_roll : public juce::MidiKeyboardComponent {
  public:
    piano_roll(MidiKeyboardState &state, Orientation orientation);

  protected:
    virtual void drawWhiteNote(int midiNoteNumber, Graphics &g,
                               Rectangle<float> area, bool isDown, bool isOver,
                               Colour lineColour, Colour textColour) override;

    virtual void drawBlackNote(int midiNoteNumber, Graphics &g,
                               Rectangle<float> area, bool isDown, bool isOver,
                               Colour noteFillColour) override;
};

class piano_synth : public juce::Synthesiser {
    // TODO: create a very simple sample-based piano
};
} // namespace jelodyne::piano
