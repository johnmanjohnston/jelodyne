#include "noteComponent.h"

jelodyne::NoteComponent::NoteComponent() : juce::Component() {}

void jelodyne::NoteComponent::paint(juce::Graphics &g) {
    g.setColour(juce::Colours::red);
    g.fillRect(getLocalBounds()); // i am embarrased at how much time it took me
                                  // to find out i should use getLocalBounds()
                                  // instead of getBounds()
}

void jelodyne::NoteComponent::mouseEnter(const juce::MouseEvent &event) {
    DBG("notecomponent mouseenter event detected");
}

void jelodyne::NoteComponent::mouseExit(const juce::MouseEvent &event) {
    DBG("mouse exit");
}
