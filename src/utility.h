#pragma once
#include <JuceHeader.h>

// TODO: move this from namespace `jelodyne` to `jelodyne::utility`
namespace jelodyne {
int noteNameToNumber(juce::String keyName, const int octaveForMiddleC);
int getYCoordinateForNote(int noteNumber, int endNote);
} // namespace jelodyne
