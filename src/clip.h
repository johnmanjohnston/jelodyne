/*
  ==============================================================================

    clip.h
    Created: 9 Oct 2024 8:00:46pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace jelodyne {
    class clip {
    public:
        juce::String path;
        
        int timestamp;
        int gain;
    };
};