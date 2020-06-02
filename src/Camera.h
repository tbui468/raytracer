#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera {
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float fov, float aspect) {
        Vec3 u, v, w;
        float theta = fov * PI / 180.0f;
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = halfHeight * aspect;
        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        m_origin = lookFrom;
        m_horizontal = 2.0f * halfWidth * u;
        m_vertical = 2.0f * halfHeight * v;
        //m_lowerLeftCorner = Vec3(-halfWidth, -halfHeight, -1.0f); 
        m_lowerLeftCorner = m_origin - halfWidth * u - halfHeight * v - w;
    }

    Ray getRay(float u, float v) {
        return Ray(m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin);
    }

    Vec3 m_origin;
    Vec3 m_horizontal;
    Vec3 m_vertical;
    Vec3 m_lowerLeftCorner;

};

#endif //CAMERA_H