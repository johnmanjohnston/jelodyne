#pragma once
#include <JuceHeader.h>

namespace jelodyne {
class shifter {
  public:
    static void shift(juce::AudioBuffer<float> &buffer,
                      const juce::AudioBuffer<float> orginalBuffer,
                      float shiftAmount);
};
} // namespace jelodyne
