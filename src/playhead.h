#pragma once
#include <JuceHeader.h>

namespace jelodyne {
class playhead : public juce::Component {
  public:
    playhead();
    int position = -1;
    void paint(juce::Graphics &g) override;
};
} // namespace jelodyne
