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
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;

    jelodyne::note noteData;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteComponent)
};
} // namespace jelodyne
