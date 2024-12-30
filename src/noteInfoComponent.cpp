#include "noteInfoComponent.h"

jelodyne::NoteInfoComponent::NoteInfoComponent() : juce::Component() {
    DBG("noteInfoComponent created ");
}
jelodyne::NoteInfoComponent::~NoteInfoComponent() {
    DBG("NOTE INFO COMPONENT DESTROYEEED");
    this->noteComponent = nullptr;
}

void jelodyne::NoteInfoComponent::paint(juce::Graphics &g) {
    // TODO: correct this background gradient
    juce::ColourGradient bg = juce::ColourGradient::vertical(
        juce::Colour(40, 40, 40), juce::Colours::black, getLocalBounds());

    // :sparkles: colors :sparkles:
    g.setGradientFill(bg);
    g.fillRect(getLocalBounds());

    g.setColour(juce::Colour(82, 82, 82));
    g.drawRect(getLocalBounds(), 2);

    // get text to draw
    juce::String editedNoteName = juce::MidiMessage::getMidiNoteName(
        this->noteComponent->noteData.noteNumber, true, true, 3);

    juce::String orginalNoteFrequency =
        juce::String(this->noteComponent->orginalNoteData.originalFrequency);

    juce::String orginalNoteName = juce::MidiMessage::getMidiNoteName(
        this->noteComponent->orginalNoteData.noteNumber, true, true, 3);

    // position and draw text

    g.setColour(juce::Colour(200, 200, 200));
    g.setFont(getInterBoldItalic());
    g.setFont(19.f);

    auto defaultBounds = getLocalBounds();
    defaultBounds.setX(defaultBounds.getX() + 5);

    g.drawText(juce::String("orginal: " + orginalNoteName + " // " +
                            orginalNoteFrequency + "Hz"),
               juce::Rectangle<float>(
                   defaultBounds.getX(), defaultBounds.getY(),
                   defaultBounds.getWidth(), defaultBounds.getHeight() / 2.f),
               juce::Justification::left, false);

    g.drawText(juce::String("edited: " + editedNoteName),
               juce::Rectangle<float>(
                   defaultBounds.getX(), defaultBounds.getY(),
                   defaultBounds.getWidth(), defaultBounds.getHeight() / 0.8f),
               juce::Justification::left, false);
}

void jelodyne::NoteInfoComponent::setCorrespondingNoteComponent(
    jelodyne::NoteComponent *nc) {
    this->noteComponent = nc;

    int width = 180;
    int height = 60;

    juce::Rectangle<int> ncBounds = nc->getBoundsInParent();
    setBounds(ncBounds.getX() - (width / 2), ncBounds.getY() - (height + 10),
              width, height);

    repaint();
}

juce::Font jelodyne::NoteInfoComponent::getInterBoldItalic() {
    auto typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::Inter_24ptBoldItalic_ttf,
        BinaryData::Inter_24ptBoldItalic_ttfSize);

    return Font(FontOptions(typeface).withHeight(16.f));
}
