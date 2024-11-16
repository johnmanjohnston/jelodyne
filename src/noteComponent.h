#pragma once
#include "jlistener/jlistenerbroadcaster.h"
#include "note.h"
#include <JuceHeader.h>

namespace jelodyne {
class NoteComponent
    : public juce::Component /*, public juce::ChangeBroadcaster*/,
      public JBroadcaster {
  public:
    NoteComponent();

    juce::Rectangle<int> componentPosition;
    void paint(juce::Graphics &g) override;
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void mouseWheelMove(const juce::MouseEvent &event,
                        const juce::MouseWheelDetails &wheel) override;

    jelodyne::note noteData;
    jelodyne::note orginalNoteData;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteComponent)
};
} // namespace jelodyne
