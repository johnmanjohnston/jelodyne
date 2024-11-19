#pragma once
#include <JuceHeader.h>

// tonality
#define MAJOR 0xC001
#define MINOR 0xC002
#define NO_TONALITY 0xC000

// key
#define KEY_C 60
#define KEY_Db 61
#define KEY_D 62
#define KEY_Eb 63
#define KEY_E 64
#define KEY_F 65
#define KEY_Gb 66
#define KEY_G 67
#define KEY_Ab 68
#define KEY_A 69
#define KEY_Bb 70
#define KEY_B 71
#define KEY_NO_KEY 0x67

// scale
#define KEY_SCALE                                                              \
    0xCC01 // TODO: from a musical point of view, I'm not sure if this is a
           // thing. Change this if it isn't
#define PENTATONIC 0xCC02
#define NO_SCALE 0xCC00

namespace jelodyne {
class scale {
  public:
    int key = KEY_NO_KEY;
    int tonality = NO_TONALITY;
    int scale = NO_SCALE;

    bool isNoteInScale(int noteNumber);
    void updateScale(int newKey, int newTonality, int newScale);

  private:
    juce::String getName(int noteNumber);

    juce::String notesInScale[7];
};
} // namespace jelodyne
