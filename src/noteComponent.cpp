#include "noteComponent.h"
#include "common.h"
#include "juce_graphics/juce_graphics.h"
#include "utility.h"

jelodyne::NoteComponent::NoteComponent()
    : juce::Component() /*, juce::ChangeBroadcaster()*/ {}

void jelodyne::NoteComponent::paint(juce::Graphics &g) {
    g.fillAll(juce::Colours::white);

    g.setColour(juce::Colours::red);
    g.fillRect(getLocalBounds()); // i am embarrased at how much time it took me
                                  // to find out i should use getLocalBounds()
                                  // instead of getBounds()
}

void jelodyne::NoteComponent::mouseDown(const juce::MouseEvent &event) {
    // send start sample number and end sample number
    broadcastMessage((void *)(uintptr_t)this->noteData.startSample,
                     (void *)(uintptr_t)TYPECODE_NOTE_START_SAMPLE);

    broadcastMessage((void *)(uintptr_t)this->noteData.endSample,
                     (void *)(uintptr_t)TYPECODE_NOTE_END_SAMPLE);
}

void jelodyne::NoteComponent::mouseUp(const juce::MouseEvent &event) {
    broadcastMessage(NULL, (void *)(uintptr_t)TYPECODE_CLEAR_NOTE_SAMPLES);
}

// obviously, moving notes by mouse like this isn't user friendly at all--this
// is just temporary
void jelodyne::NoteComponent::mouseWheelMove(
    const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel) {
    if (wheel.deltaY > 0.f)
        noteData.noteNumber++;
    else
        noteData.noteNumber--;

    // find a better name for this later
    juce::Rectangle<int> boundsWithinParentBeforeUpdating = getBoundsInParent();

    int updatedYCoordinate =
        jelodyne::getYCoordinateForNote(noteData.noteNumber, 12 * (5 + 2));

    setBounds(getX(), updatedYCoordinate, getWidth(), getHeight());

    repaint();
    getParentComponent()->repaint(boundsWithinParentBeforeUpdating);
}
