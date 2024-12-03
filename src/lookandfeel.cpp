#include "lookandfeel.h"

jelodyne::JelodyneLookAndFeel::JelodyneLookAndFeel() {
    // set ComboBox colors
    setColour(juce::ComboBox::ColourIds::focusedOutlineColourId,
              Palette::getElementBg()); // set focused outline

    setColour(juce::ComboBox::ColourIds::textColourId,
              Palette::getElementText()); // set text color

    setColour(juce::ComboBox::ColourIds::arrowColourId,
              Palette::getElementText()); // set arrow color

    setColour(juce::PopupMenu::ColourIds::backgroundColourId,
              Palette::getElementBg().brighter(.1f)); // set dropdown color

    setColour(juce::ComboBox::ColourIds::outlineColourId,
              Palette::getElementOutline()); // set outline

    // set TextButton colors
    setColour(juce::TextButton::ColourIds::textColourOnId,
              Palette::getElementText()); // set text color for on

    setColour(juce::TextButton::ColourIds::textColourOffId,
              Palette::getElementText()); // set text color for off
}

juce::Font jelodyne::JelodyneLookAndFeel::getInterBoldItalic() {

    auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Inter_24ptBoldItalic_ttf,
        BinaryData::Inter_24ptBoldItalic_ttfSize);

    return Font(FontOptions(typeface).withHeight(16.f));
}

juce::Font jelodyne::JelodyneLookAndFeel::getPopupMenuFont() {
    return getInterBoldItalic();
}

juce::Font jelodyne::JelodyneLookAndFeel::getComboBoxFont(juce::ComboBox &box) {
    return getInterBoldItalic();
}

// https://github.com/juce-framework/JUCE/blob/master/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.cpp#L290
void jelodyne::JelodyneLookAndFeel::drawButtonBackground(
    juce::Graphics &g, juce::Button &button,
    const juce::Colour &backgroundColour, bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown) {
    auto cornerSize = 0.f;
    auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

    auto baseColour =
        backgroundColour
            .withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f
                                                                    : 0.9f)
            .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

    if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
        baseColour =
            baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

    g.setColour(baseColour);

    auto flatOnLeft = button.isConnectedOnLeft();
    auto flatOnRight = button.isConnectedOnRight();
    auto flatOnTop = button.isConnectedOnTop();
    auto flatOnBottom = button.isConnectedOnBottom();

    g.setGradientFill(Palette::getElementBgGradient(bounds));

    if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom) {
        Path path;
        path.addRoundedRectangle(
            bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
            cornerSize, cornerSize, !(flatOnLeft || flatOnTop),
            !(flatOnRight || flatOnTop), !(flatOnLeft || flatOnBottom),
            !(flatOnRight || flatOnBottom));

        g.fillPath(path);

        g.setColour(button.findColour(ComboBox::outlineColourId));
        g.strokePath(path, PathStrokeType(2.0f));
    } else {
        g.fillRoundedRectangle(bounds, cornerSize);

        g.setColour(button.findColour(ComboBox::outlineColourId));
        g.drawRoundedRectangle(bounds, cornerSize, 2.0f);
    }
}

juce::Font jelodyne::JelodyneLookAndFeel::getTextButtonFont(juce::TextButton &,
                                                            int buttonHeight) {
    return getInterBoldItalic();
}

// https://github.com/juce-framework/JUCE/blob/master/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.cpp#L929
void jelodyne::JelodyneLookAndFeel::drawComboBox(Graphics &g, int width,
                                                 int height, bool isButtonDown,
                                                 int buttonX, int buttonY,
                                                 int buttonW, int buttonH,
                                                 ComboBox &box) {
    Rectangle<int> boxBounds(0, 0, width, height);

    g.setGradientFill(Palette::getElementBgGradient(boxBounds.toFloat()));
    g.fillRect(boxBounds);

    g.setColour(box.findColour(ComboBox::outlineColourId));
    g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), 0.f, 2.0f);

    Rectangle<int> arrowZone(width - 30, 0, 20, height);
    Path path;
    path.startNewSubPath((float)arrowZone.getX() + 3.0f,
                         (float)arrowZone.getCentreY() - 2.0f);
    path.lineTo((float)arrowZone.getCentreX(),
                (float)arrowZone.getCentreY() + 3.0f);
    path.lineTo((float)arrowZone.getRight() - 3.0f,
                (float)arrowZone.getCentreY() - 2.0f);

    g.setColour(box.findColour(ComboBox::arrowColourId)
                    .withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
    g.strokePath(path, PathStrokeType(2.0f));
}
