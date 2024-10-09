/*
  ==============================================================================

    project.h
    Created: 9 Oct 2024 8:03:07pm
    Author:  USER

  ==============================================================================
*/

#pragma once
#include <vector>
#include "track.h";

namespace jelodyne {
    class project {
        std::vector<jelodyne::track> tracks;
    };
}