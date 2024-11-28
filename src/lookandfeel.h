#pragma once
#include <JuceHeader.h>

namespace jelodyne {
class JelodyneLookAndFeel : public juce::LookAndFeel_V4 {
  public:
    JelodyneLookAndFeel();
    Font getInterBoldItalic();

    Font getPopupMenuFont() override;
    Font getComboBoxFont(juce::ComboBox &box) override;

    void drawComboBox(Graphics &g, int width, int height, bool isButtonDown,
                      int buttonX, int buttonY, int buttonW, int buttonH,
                      ComboBox &box) override;
};
} // namespace jelodyne
