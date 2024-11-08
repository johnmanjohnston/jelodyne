#include "note.h"

void jelodyne::consolidateDuplicateNotes(std::vector<jelodyne::note> &notes) {
    // DBG("! consolidate() called");

    for (std::vector<jelodyne::note>::size_type i = 0; i != notes.size(); i++) {
        if (i + 1 != notes.size()) {
            if (notes[i].noteNumber == notes[i + 1].noteNumber) {
                notes.erase(notes.begin() + (long)i + 1);
                notes[i].endSample = notes[i + 1].startSample;
            }
        }
    }
}

void jelodyne::removePitchArtifacts(std::vector<note> &notes, int fft_size) {
    for (std::vector<jelodyne::note>::size_type i = 0; i != notes.size(); i++) {
        if (notes[i].endSample - notes[i].startSample == fft_size) {
            if (i + 1 != notes.size())
                notes[i + 1].startSample = notes[i].startSample;

            notes.erase(notes.begin() + (long)i);
        }
    }
}
