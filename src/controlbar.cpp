#include "controlbar.h"
#include "common.h"
#include "scale.h"

jelodyne::ControlBar::ControlBar() : juce::Component() {
    filenameLabel.setColour(juce::Label::textColourId,
                            juce::Colours::lightgrey);

    filenameLabel.setFont(getInterBold());
    addAndMakeVisible(filenameLabel);

    // scale selection
    for (int i = 0; i <= 12; ++i) {
        juce::String noteName =
            juce::MidiMessage::getMidiNoteName(i + 59, true, false, 1);

        // if it's sharp add the flat name also (ex., "C#/Db")
        if (juce::MidiMessage::isMidiNoteBlack(i + 59))
            noteName.append("/" + juce::MidiMessage::getMidiNoteName(
                                      i + 59, false, false, 1),
                            3);

        keySelectorBox.addItem(noteName, i + 1);
    }

    tonalitySelectorBox.addItem("Major", 1);
    tonalitySelectorBox.addItem("Minor", 2);

    scaleSelectorBox.addItem("Key Scale", 1);
    scaleSelectorBox.addItem("Pentatonic", 2);

    addAndMakeVisible(keySelectorBox);
    addAndMakeVisible(tonalitySelectorBox);
    addAndMakeVisible(scaleSelectorBox);

    keySelectorBox.onChange = [this] { onScalesSelectorBoxesChange(); };
    tonalitySelectorBox.onChange = [this] { onScalesSelectorBoxesChange(); };
    scaleSelectorBox.onChange = [this] { onScalesSelectorBoxesChange(); };
}

void jelodyne::ControlBar::onScalesSelectorBoxesChange() {
    int newKey = keySelectorBox.getSelectedId() + 59 - 1;
    int newTonality = tonalitySelectorBox.getSelectedId() == 1 ? MAJOR : MINOR;
    int newScale =
        scaleSelectorBox.getSelectedId() == 1 ? KEY_SCALE : PENTATONIC;

    scale updatedScale;
    updatedScale.updateScale(newKey, newTonality, newScale);

    broadcastMessage((void *)&updatedScale,
                     (void *)(uintptr_t)TYPECODE_UPDATED_SCALE_DATA);
}

void jelodyne::ControlBar::paint(juce::Graphics &g) {
    g.fillAll(juce::Colour(25, 25, 25));

    filenameLabel.setText("insert filename here.jlp",
                          juce::NotificationType::dontSendNotification);
}

void jelodyne::ControlBar::resized() {
    filenameLabel.setBounds(getLocalBounds().getX(), getLocalBounds().getY(),
                            256, getLocalBounds().getHeight());

    auto lb = getLocalBounds();
    keySelectorBox.setBounds(lb.getX() + 400, lb.getY() + 5, 100,
                             lb.getHeight() - 10);

    tonalitySelectorBox.setBounds(lb.getX() + 510, lb.getY() + 5, 100,
                                  lb.getHeight() - 10);

    scaleSelectorBox.setBounds(lb.getX() + 620, lb.getY() + 5, 100,
                               lb.getHeight() - 10);
}

juce::Font jelodyne::ControlBar::getInterBold() {
    auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Inter_24ptBoldItalic_ttf,
        BinaryData::Inter_24ptBoldItalic_ttfSize);

    return Font(FontOptions(typeface).withHeight(16.f));
}
