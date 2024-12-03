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
};

class Palette {
  public:
    static juce::Colour getPurple() { return juce::Colour(182, 63, 255); }

    static juce::Colour getElementBg() { return juce::Colour(20, 20, 20); }
    static juce::Colour getElementText() { return juce::Colour(200, 200, 200); }
    static juce::Colour getElementOutline() { return juce::Colour(65, 65, 65); }

    static juce::ColourGradient
    getElementBgGradient(juce::Rectangle<float> bounds) {
        return juce::ColourGradient::vertical(getElementBg().brighter(.05f),
                                              getElementBg(), bounds);
    }
};
} // namespace jelodyne
