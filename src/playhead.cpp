#include "playhead.h"

jelodyne::playhead::playhead() : juce::Component() {}

void jelodyne::playhead::paint(juce::Graphics &g) {
    // TODO this function
    g.setColour(juce::Colours::green);
    g.fillRect(this->getLocalBounds());
}
