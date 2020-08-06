#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns random real number in [0, 1[;
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns random real number in [min, max[;
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min) x = min;
    if (x > max) x = max;
    return x;
}

// Common headers
#include "ray.h"
#include "vec3.h"

#endif