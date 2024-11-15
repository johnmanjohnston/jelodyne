#include "shift.h"

void jelodyne::shifter::shift(juce::AudioBuffer<float> &buffer,
                              float shiftAmount) {
    DBG("shift() called");
    juce::Random random;

    for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
        auto *bufWriter = buffer.getWritePointer(channel, 0);

        for (auto sample = 0; sample < buffer.getNumSamples(); ++sample) {
            bufWriter[sample] = random.nextFloat() * .25f - .125f;
        }
    }
}
