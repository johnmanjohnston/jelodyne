#include "noteComponent.h"

jelodyne::NoteComponent::NoteComponent() {}

void jelodyne::NoteComponent::paint(juce::Graphics &g) {
    DBG("paint called on NoteComponent instance");
    g.setColour(juce::Colours::red);
    g.fillRect(getX(), getY(), getWidth(), getHeight());
}

void jelodyne::NoteComponent::mouseEnter(const juce::MouseEvent &event) {
    DBG("notecomponent mouseenter event detected");
}
