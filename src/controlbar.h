#pragma once
#include "jlistener/jlistenerbroadcaster.h"
#include <JuceHeader.h>

namespace jelodyne {
class ControlBar : public juce::Component, public JBroadcaster {
  public:
    ControlBar();
    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    juce::Label filenameLabel;
    juce::Label doYouSuckAtSingingLabel;

    // scale selection
    juce::ComboBox keySelectorBox;
    juce::ComboBox tonalitySelectorBox;
    juce::ComboBox scaleSelectorBox;
    void onScalesSelectorBoxesChange();

    // other buttons
    juce::TextButton quitButton;
    juce::TextButton exportButton;
    juce::TextButton saveButton;
    juce::TextButton miscButton;

    juce::Font getInterBold();
};
} // namespace jelodyne
