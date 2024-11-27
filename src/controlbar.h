#pragma once
#include <JuceHeader.h>

namespace jelodyne {
class ControlBar : public juce::Component {
  public:
    ControlBar();
    void paint(juce::Graphics &g) override;
    void resized() override;

  private:
    juce::Label filenameLabel;

    // scale selection
    juce::ComboBox keySelectorBox;
    juce::ComboBox tonalitySelectorBox;
    juce::ComboBox scaleSelectorBox;
    void onScalesSelectorBoxesChange();

    juce::Font getInterBold();
};
} // namespace jelodyne
