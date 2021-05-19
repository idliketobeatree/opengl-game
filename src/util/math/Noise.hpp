#pragma once
#include "Vector.hpp"

/**
 * Adapted to C++ from Heikki Törmälä's C# implementation of simplex noise,
 * hosted at https://github.com/Xpktro/simplexnoise
 */

class Noise {
public:
    float noise1f(float x);
    float noise2f(float x, float y);
    float noise3f(float x, float y, float z);
};