#include "note.h"

void jelodyne::consolidate_duplicate_notes(std::vector<jelodyne::note> notes) {
    // DBG("! consolidate() called");

    for (std::vector<jelodyne::note>::size_type i = 0; i != notes.size(); i++) {
        // TODO: add an end element property to all notes
        // TODO: check if next element is same note as current element. if it
        // is, remove the next element and update the end sample of this element
    }
}
