#ifndef CAMERA_H
#define CAMERA_H


Vec3 random_in_unit_disk() {
    Vec3 p;
    do {
        p = Vec3(randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), 0.0f);
    }while(dot(p, p) >= 1.0f);
    return p;
}

class Camera {
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vup, float fov, float aspect, float aperture, float focus_distance, float timeStart, float timeEnd):
                m_time0(timeStart), m_time1(timeEnd) {
        m_lenseRadius = aperture / 2.0f;
        float theta = degrees_to_radians(fov);
        float halfHeight = tan(theta / 2.0f);
        float halfWidth = halfHeight * aspect;
        w = unit_vector(lookFrom - lookAt);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        m_origin = lookFrom;
        m_horizontal = 2.0f * halfWidth * u * focus_distance;
        m_vertical = 2.0f * halfHeight * v * focus_distance;
        m_lowerLeftCorner = m_origin - halfWidth * u * focus_distance - halfHeight * v * focus_distance - w * focus_distance;
    }

    Ray getRay(float s, float t) {
        Vec3 rd = m_lenseRadius * random_in_unit_disk();
        Vec3 offset = u * rd.x() + v * rd.y();
        return Ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset, randf(m_time0, m_time1));
    }

    Vec3 m_origin;
    Vec3 m_horizontal;
    Vec3 m_vertical;
    Vec3 m_lowerLeftCorner;
    Vec3 u, v, w;
    float m_lenseRadius;
    float m_time0;
    float m_time1;
};

#endif //CAMERA_H