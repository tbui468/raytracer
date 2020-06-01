#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera {
public:
    Camera() {
        m_origin = Vec3(0.0f, 0.0f, 0.0f);
        m_horizontal = Vec3(4.0f, 0.0f, 0.0f);
        m_vertical = Vec3(0.0f, 2.0f, 0.0f);
        m_lowerLeftCorner = Vec3(-2.0f, -1.0f, -1.0f); //lower left corner of screen
    }

    Ray getRay(float u, float v) {
        return Ray(m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical);
    }

    Vec3 m_origin;
    Vec3 m_horizontal;
    Vec3 m_vertical;
    Vec3 m_lowerLeftCorner;

};

#endif //CAMERA_H