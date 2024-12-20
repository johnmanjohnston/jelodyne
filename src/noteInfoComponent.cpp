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

    g.setGradientFill(bg);
    g.fillRect(getLocalBounds());

    g.setColour(juce::Colour(82, 82, 82));
    g.drawRect(getLocalBounds(), 2);

    g.setColour(juce::Colours::white);
    g.setFont(17.f);

    juce::String noteNumberAsStr =
        juce::String(this->noteComponent->noteData.noteNumber);

    g.drawText(noteNumberAsStr, getLocalBounds(), juce::Justification::left,
               false);
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
