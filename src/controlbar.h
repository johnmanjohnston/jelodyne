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

    juce::Font getInterBold();
};
} // namespace jelodyne
