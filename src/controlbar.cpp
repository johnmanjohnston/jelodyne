#include "controlbar.h"

jelodyne::ControlBar::ControlBar() : juce::Component() {
    filenameLabel.setColour(juce::Label::textColourId,
                            juce::Colours::lightgrey);
    filenameLabel.setFont(getInterBold());
    addAndMakeVisible(filenameLabel);
}

void jelodyne::ControlBar::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::black);

    filenameLabel.setText("insert filename here.jlp",
                          juce::NotificationType::dontSendNotification);
}

void jelodyne::ControlBar::resized() {
    filenameLabel.setBounds(getLocalBounds().getX(), getLocalBounds().getY(),
                            256, getLocalBounds().getHeight());
}

juce::Font jelodyne::ControlBar::getInterBold() {
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Inter_24ptBoldItalic_ttf,
        BinaryData::Inter_24ptBoldItalic_ttfSize);
    ScopedPointer<Font> font = new Font(typeface);
    font->setHeight(18.f);

    return *font;
}
