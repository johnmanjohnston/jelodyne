#include "noteComponent.h"
#include "common.h"
#include "shift.h"
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
    broadcastMessage((void *)&noteData, (void *)(uintptr_t)TYPECODE_NOTE);
}

void jelodyne::NoteComponent::mouseUp(const juce::MouseEvent &event) {
    broadcastMessage(NULL, (void *)(uintptr_t)TYPECODE_CLEAR_NOTE);
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

    shifter::shift(noteData.buffer, 0.f);
}
