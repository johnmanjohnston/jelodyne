#include "shift.h"
#include "rubberband/RubberBandStretcher.h"

void jelodyne::shifter::shift(juce::AudioBuffer<float> &buffer,
                              const juce::AudioBuffer<float> orginalBuffer,
                              float shiftAmount) {
    buffer = orginalBuffer;

    if (shiftAmount == 0.f) {
        return;
    }

    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();

    // so that rubberband can process it
    std::vector<const float *> inputChannels;
    for (int channel = 0; channel < numChannels; ++channel) {
        inputChannels.push_back(buffer.getReadPointer(channel));
    }

    // initiailize rubberband and process audio
    // TODO: sampleRate is hardcoded at 44100 here--change that later
    RubberBand::RubberBandStretcher stretcher(
        44100, (size_t)numChannels,
        RubberBand::RubberBandStretcher::OptionProcessRealTime |
            RubberBand::RubberBandStretcher::OptionPitchHighQuality);

    stretcher.setPitchScale(std::pow(2, shiftAmount / 12.f));

    stretcher.process(inputChannels.data(), (size_t)numSamples, false);

    // write to buffer
    std::vector<float *> outputChannels;
    for (int ch = 0; ch < numChannels; ++ch) {
        outputChannels.push_back(buffer.getWritePointer(ch));
    }

    const int availableSamples = stretcher.available();
    stretcher.retrieve(outputChannels.data(), (size_t)availableSamples);
}
