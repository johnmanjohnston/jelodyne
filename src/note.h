#pragma once
#include "JuceHeader.h"
#include <vector>
namespace jelodyne {
class note {
  public:
    int noteNumber = -1;
    int startSample = -1;
    int endSample = -1;
    int originalFrequency = -1;
};

void consolidateDuplicateNotes(std::vector<note> &notes);
void removePitchArtifacts(std::vector<note> &notes, int fft_size);
} // namespace jelodyne
