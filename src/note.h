#pragma once
#include "JuceHeader.h"
#include <vector>
namespace jelodyne {
class note {
  public:
    juce::String note_name;
    int start_sample;
};

void consolidate_duplicate_notes(std::vector<note> notes);
} // namespace jelodyne
