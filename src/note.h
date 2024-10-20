#pragma once
#include "JuceHeader.h"
#include <vector>
namespace jelodyne {
class note {
  public:
    int note_number = -1;
    int start_sample = -1;
    int end_sample = -1;
    int original_frequency = -1;
};

void consolidate_duplicate_notes(std::vector<note> &notes);
void remove_pitch_artifacts(std::vector<note> &notes, int fft_size);
} // namespace jelodyne
