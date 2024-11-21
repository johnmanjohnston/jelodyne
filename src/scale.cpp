#include "scale.h"

void jelodyne::scale::updateScale(int newKey, int newTonality, int newScale) {
    this->tonality = newTonality;
    this->key = newKey;
    this->scale = newScale;

    if (this->tonality == MINOR) {
        if (this->scale == PENTATONIC) {
            notesInScale[0] = getName(key);
            notesInScale[1] = getName(key + 3);
            notesInScale[2] = getName(key + 5);
            notesInScale[3] = getName(key + 7);
            notesInScale[4] = getName(key + 10);
            notesInScale[5] = "";
            notesInScale[6] = "";
        }
    }

    if (this->tonality == MAJOR) {
        if (this->scale == KEY_SCALE) {
            notesInScale[0] = getName(key);
            notesInScale[1] = getName(key + 2);
            notesInScale[2] = getName(key + 4);
            notesInScale[3] = getName(key + 5);
            notesInScale[4] = getName(key + 7);
            notesInScale[5] = getName(key + 9);
            notesInScale[6] = getName(key + 11);
        }
    }

    if (this->tonality == MINOR) {
        if (this->scale == KEY_SCALE) {
            notesInScale[0] = getName(key);
            notesInScale[1] = getName(key + 2);
            notesInScale[2] = getName(key + 3);
            notesInScale[3] = getName(key + 5);
            notesInScale[4] = getName(key + 7);
            notesInScale[5] = getName(key + 8);
            notesInScale[6] = getName(key + 10);
        }
    }
}

// to shorten stuff later
juce::String jelodyne::scale::getName(int noteNumber) {
    return juce::MidiMessage::getMidiNoteName(noteNumber, false, false, 4);
}

bool jelodyne::scale::isNoteInScale(int noteNumber) {
    // TODO: this is probably a really inefficient way to check if a note is
    // in the right scale, but this is now a problem for future you
    bool retval = false;

    juce::String currentNoteName =
        juce::MidiMessage::getMidiNoteName(noteNumber, false, false, 4);

    for (int i = 0; (size_t)i < sizeof(notesInScale) / sizeof(juce::String);
         ++i) {
        if (currentNoteName == notesInScale[i]) {
            retval = true;
            break;
        }
    }

    return retval;
}
