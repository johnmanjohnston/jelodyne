#include "note.h"

void jelodyne::consolidate_duplicate_notes(std::vector<jelodyne::note> &notes) {
    // DBG("! consolidate() called");

    for (std::vector<jelodyne::note>::size_type i = 0; i != notes.size(); i++) {
        // TODO: check if next element is same note as current element. if it
        // is, remove the next element and update the end sample of this element
        //

        if (i + 1 != notes.size()) {
            if (notes[i].note_number == notes[i + 1].note_number) {
                notes.erase(notes.begin() + (long)i + 1);
                notes[i].end_sample = notes[i + 1].start_sample;
            }
        }
    }
}
