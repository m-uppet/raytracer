#ifndef MY_STB_IMAGE_H
#define MY_STB_IMAGE_H

// Disable "pedantic" warnings for this library.
#ifdef _MSC_VER // Microsoft Visual C++ compiler
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

// Restore warning levels.
#ifdef MSC_VER
    #pragma warning (pop)
#endif

#endif