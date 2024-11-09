#include "noteComponent.h"
#include "common.h"

jelodyne::NoteComponent::NoteComponent()
    : juce::Component() /*, juce::ChangeBroadcaster()*/ {}

void jelodyne::NoteComponent::paint(juce::Graphics &g) {
    g.setColour(juce::Colours::red);
    g.fillRect(getLocalBounds()); // i am embarrased at how much time it took me
                                  // to find out i should use getLocalBounds()
                                  // instead of getBounds()
}

void jelodyne::NoteComponent::mouseEnter(const juce::MouseEvent &event) {
    // send start sample
    broadcastMessage((void *)(uintptr_t)this->noteData.startSample,
                     (void *)(uintptr_t)TYPECODE_NOTE_START_SAMPLE);

    // send end sample
    broadcastMessage((void *)(uintptr_t)this->noteData.endSample,
                     (void *)(uintptr_t)TYPECODE_NOTE_END_SAMPLE);
}

void jelodyne::NoteComponent::mouseExit(const juce::MouseEvent &event) {
    // DBG("mouse exit");
    broadcastMessage(NULL, (void *)(uintptr_t)TYPECODE_CLEAR_NOTE_SAMPLES);
}
