#include "note.h"

void jelodyne::consolidate_duplicate_notes(std::vector<jelodyne::note> &notes) {
    // DBG("! consolidate() called");

    for (std::vector<jelodyne::note>::size_type i = 0; i != notes.size(); i++) {
        if (i + 1 != notes.size()) {
            if (notes[i].note_number == notes[i + 1].note_number) {
                notes.erase(notes.begin() + (long)i + 1);
                notes[i].end_sample = notes[i + 1].start_sample;
            }
        }
    }
}

void jelodyne::remove_pitch_artifacts(std::vector<note> &notes, int fft_size) {
    for (std::vector<jelodyne::note>::size_type i = 0; i != notes.size(); i++) {
        if (notes[i].end_sample - notes[i].start_sample == fft_size) {
            if (i + 1 != notes.size())
                notes[i + 1].start_sample = notes[i].start_sample;

            notes.erase(notes.begin() + (long)i);
        }
    }
}
