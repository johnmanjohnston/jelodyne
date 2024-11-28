#include "lookandfeel.h"
#include "juce_graphics/juce_graphics.h"

jelodyne::JelodyneLookAndFeel::JelodyneLookAndFeel() {
    juce::Colour dropdownBg = juce::Colour(20, 20, 20);
    setColour(juce::ComboBox::ColourIds::backgroundColourId, dropdownBg);
    setColour(juce::ComboBox::ColourIds::focusedOutlineColourId, dropdownBg);
    setColour(juce::ComboBox::ColourIds::textColourId,
              juce::Colour(200, 200, 200));
    setColour(juce::PopupMenu::ColourIds::backgroundColourId,
              dropdownBg.brighter(.1f));

    setColour(juce::ComboBox::ColourIds::outlineColourId,
              juce::Colour(65, 65, 65));
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

// https://github.com/juce-framework/JUCE/blob/master/modules/juce_gui_basics/lookandfeel/juce_LookAndFeel_V4.cpp#L929
void jelodyne::JelodyneLookAndFeel::drawComboBox(Graphics &g, int width,
                                                 int height, bool isButtonDown,
                                                 int buttonX, int buttonY,
                                                 int buttonW, int buttonH,
                                                 ComboBox &box) {
    Rectangle<int> boxBounds(0, 0, width, height);

    juce::ColourGradient gradient = juce::ColourGradient::vertical(
        box.findColour(ComboBox::backgroundColourId).brighter(.05f),
        box.findColour(ComboBox::backgroundColourId), boxBounds);

    g.setColour(box.findColour(ComboBox::backgroundColourId));
    g.setGradientFill(gradient);
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
