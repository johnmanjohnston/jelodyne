#pragma once
#include <JuceHeader.h>

namespace jelodyne {
class shifter {
  public:
    static void shift(juce::AudioBuffer<float> &buffer, float shiftAmount);
};
} // namespace jelodyne
