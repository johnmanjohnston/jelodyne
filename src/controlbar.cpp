#include "controlbar.h"
#include "common.h"
#include "scale.h"

jelodyne::ControlBar::ControlBar() : juce::Component() {
    filenameLabel.setColour(juce::Label::textColourId,
                            juce::Colours::lightgrey);

    filenameLabel.setFont(getInterBold());
    addAndMakeVisible(filenameLabel);

    // scale selection
    // add keys
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

    // add modes
    tonalitySelectorBox.addItem("Major", 1);
    tonalitySelectorBox.addItem("Minor", 2);

    // add scales
    scaleSelectorBox.addItem(
        "Natural", 1); // apparently "Natural" is the right term. TODO: change
                       // all references of key scale to natural in codebase
    scaleSelectorBox.addItem("Pentatonic", 2);

    addAndMakeVisible(keySelectorBox);
    addAndMakeVisible(tonalitySelectorBox);
    addAndMakeVisible(scaleSelectorBox);

    keySelectorBox.onChange = [this] { onScalesSelectorBoxesChange(); };
    tonalitySelectorBox.onChange = [this] { onScalesSelectorBoxesChange(); };
    scaleSelectorBox.onChange = [this] { onScalesSelectorBoxesChange(); };

    keySelectorBox.setText("Key");
    tonalitySelectorBox.setText("Mode"); // apparently it can also be called a
                                         // "mode"? idk enough about music, man
    scaleSelectorBox.setText("Scale");

    // other buttons
    quitButton.setButtonText("Quit");
    exportButton.setButtonText("Export");
    saveButton.setButtonText("Save");
    miscButton.setButtonText("Misc.");

    miscButton.onClick = [] { DBG("misc button clicked"); };
    saveButton.onClick = [] { DBG("save button clicked"); };
    exportButton.onClick = [] { DBG("export button clicked"); };
    quitButton.onClick = [] { JUCEApplicationBase::quit(); };

    addAndMakeVisible(quitButton);
    addAndMakeVisible(exportButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(miscButton);

    // the do you suck at singing label
    doYouSuckAtSingingLabel.setFont(getInterBold().withHeight(14.f));
    doYouSuckAtSingingLabel.setColour(juce::Label::textColourId,
                                      juce::Colour(100, 100, 100));
    doYouSuckAtSingingLabel.setText(
        "3/4 notes corrected; wow you suck at singing",
        juce::NotificationType::dontSendNotification);

    addAndMakeVisible(doYouSuckAtSingingLabel);
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
    auto lb = getLocalBounds();
    filenameLabel.setBounds(lb.getX(), lb.getY(), 256, lb.getHeight());

    keySelectorBox.setBounds(lb.getX() + 400, lb.getY() + 5, 80,
                             lb.getHeight() - 10);

    tonalitySelectorBox.setBounds(lb.getX() + 490 - 6, lb.getY() + 5, 80,
                                  lb.getHeight() - 10);

    scaleSelectorBox.setBounds(lb.getX() + 580 - 12, lb.getY() + 5, 110,
                               lb.getHeight() - 10);

    int btnWidth = 80;
    int btnMargin = 4;
    quitButton.setBounds(lb.getWidth() - (btnWidth + btnMargin), lb.getY() + 5,
                         btnWidth, lb.getHeight() - 10);

    exportButton.setBounds(lb.getWidth() - ((btnWidth * 2) + (btnMargin * 2)),
                           lb.getY() + 5, btnWidth, lb.getHeight() - 10);

    saveButton.setBounds(lb.getWidth() - ((btnWidth * 3) + (btnMargin * 3)),
                         lb.getY() + 5, btnWidth, lb.getHeight() - 10);

    miscButton.setBounds(lb.getWidth() - ((btnWidth * 4) + (btnMargin * 4)),
                         lb.getY() + 5, btnWidth, lb.getHeight() - 10);

    doYouSuckAtSingingLabel.setBounds(lb.getWidth() / 2 + 40, lb.getY(), 300,
                                      lb.getHeight());
}

juce::Font jelodyne::ControlBar::getInterBold() {
    auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Inter_24ptBoldItalic_ttf,
        BinaryData::Inter_24ptBoldItalic_ttfSize);

    return Font(FontOptions(typeface).withHeight(16.f));
}
