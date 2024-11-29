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

    void drawButtonBackground(juce::Graphics &g, juce::Button &button,
                              const juce::Colour &backgroundColour,
                              bool shouldDrawButtonAsHighlighted,
                              bool shouldDrawButtonAsDown) override;

    juce::Font getTextButtonFont(juce::TextButton &, int buttonHeight) override;

  private:
    const juce::Colour elementBg = juce::Colour(20, 20, 20);
    const juce::Colour elementText = juce::Colour(200, 200, 200);
    const juce::Colour elementOutline = juce::Colour(65, 65, 65);
    juce::ColourGradient getElementBgGradient(juce::Rectangle<float> bounds);
};
} // namespace jelodyne
