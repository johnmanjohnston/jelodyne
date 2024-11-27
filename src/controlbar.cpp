#include "controlbar.h"

jelodyne::ControlBar::ControlBar() : juce::Component() {
    filenameLabel.setColour(juce::Label::textColourId,
                            juce::Colours::lightgrey);
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
