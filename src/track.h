/*
  ==============================================================================

    track.h
    Created: 9 Oct 2024 7:58:46pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <vector>
#include "clip.h"

namespace jelodyne {
    class track {
        std::vector<jelodyne::clip> clips;
    };
}