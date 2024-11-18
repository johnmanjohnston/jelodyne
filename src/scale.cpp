#include "scale.h"

void jelodyne::scale::updateScale(int newKey, int newTonality, int newScale) {
    this->tonality = newTonality;
    this->key = newKey;
    this->scale = newScale;
}

// to shorten stuff later
juce::String jelodyne::scale::getName(int noteNumber) {
    return juce::MidiMessage::getMidiNoteName(noteNumber, false, false, 4);
}

bool jelodyne::scale::isNoteInScale(int noteNumber) {
    // TODO: this is probably a really inefficient way to check if a note is in
    // the right scale, but this is now a problem for future you
    bool retval = false;

    // as of now, the longest scale will be major/minor scale which has 7 notes
    juce::String notesInScale[7];

    juce::String currentNoteName =
        juce::MidiMessage::getMidiNoteName(noteNumber, false, false, 4);

    if (this->tonality == MINOR) {
        if (this->scale == PENTATONIC) {
            notesInScale[0] = getName(key);
            notesInScale[1] = getName(key + 3);
            notesInScale[2] = getName(key + 5);
            notesInScale[3] = getName(key + 7);
            notesInScale[4] = getName(key + 10);
        }

        for (int i = 0; (size_t)i < sizeof(notesInScale) / sizeof(juce::String);
             ++i) {
            if (currentNoteName == notesInScale[i]) {
                retval = true;
                break;
            }
        }
    }

    return retval;
}
