#include "utility.h"

int jelodyne::noteNameToNumber(juce::String keyName,
                               const int octaveForMiddleC) {
    if (keyName == "invalid note")
        return -1;

    static const char *noteNames[] = {"C",  "C#", "D",  "D#", "E",  "F",
                                      "F#", "G",  "G#", "A",  "A#", "B"};

    int keyNumber, octave = 0, numPos = keyName.indexOfAnyOf("01234567890 -");

    if (numPos == 0)
        keyNumber = keyName.getIntValue();

    else {
        if (numPos > 0) {
            octave =
                keyName.substring(numPos).getIntValue() - octaveForMiddleC + 5;
        } else {
            octave =
                octaveForMiddleC; // default to octave of middle C if none found
            numPos = keyName.length();
        }

        String name(keyName.substring(0, numPos).trim().toUpperCase());

        keyNumber = StringArray(noteNames, 12).indexOf(name) % 12;
    }

    return keyNumber + octave * 12;
}

int jelodyne::getYCoordinateForNote(int noteNumber, int endNote) {
    float preciseCellHeight = 19.0f;
    int yValueWithoutOffset =
        ((float)((float)endNote - noteNumber) * preciseCellHeight);

    int yOffset = 14;

    return yValueWithoutOffset + yOffset;
}
