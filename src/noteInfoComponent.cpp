#include "noteInfoComponent.h"

jelodyne::NoteInfoComponent::NoteInfoComponent() : juce::Component() {
    DBG("noteInfoComponent created ");
}
jelodyne::NoteInfoComponent::~NoteInfoComponent() {
    DBG("NOTE INFO COMPONENT DESTROYEEED");
    this->noteComponent = nullptr;
}

void jelodyne::NoteInfoComponent::paint(juce::Graphics &g) {
    g.setColour(juce::Colours::pink);
    g.fillRect(getLocalBounds());

    g.setColour(juce::Colours::black);
    g.setFont(17.f);

    juce::String noteNumberAsStr =
        juce::String(this->noteComponent->noteData.noteNumber);

    g.drawText(noteNumberAsStr, getLocalBounds(), juce::Justification::left,
               false);
}

void jelodyne::NoteInfoComponent::setCorrespondingNoteComponent(
    jelodyne::NoteComponent *nc) {
    this->noteComponent = nc;
    setBounds(nc->getLocalBounds());
    repaint();
}
