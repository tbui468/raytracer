#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <random>
#include <limits>

//constants
const float infinity = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897f;

//utility functions
//returns random float in range [0, 1)
inline float randf(){
    return static_cast<float>(rand()) / RAND_MAX;
}

//returns random float in range [min, max)
inline float randf(float min, float max) {
    if(max < min)
        std::swap(max, min);
    return randf() * (max - min) + min;
}

//returns random int in range[min, max)
inline int randi(int min, int max) {
    int diff = max - min;
    return rand() % diff + min;
}

inline float degrees_to_radians(float degrees) {
    return degrees * PI / 180.0f;
}

inline float clamp(float x, float min, float max) {
    if(x < min) return min;
    if(x > max) return max;
    return x;
}

//common headers
#include "Vec3.h"
#include "Ray.h"
#include "AABB.h"
#define STB_IMAGE_IMPLEMENTATION
#include "external/stb/stb_image.h"

#endif //RAYTRACER_H