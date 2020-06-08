#ifndef AABB_H
#define AABB_H

#include "RayTracer.h"

class AABB {
public:
    AABB() {};
    AABB(const Point3& min, const Point3& max): m_min(min), m_max(max) {};
    Point3 min() const {return m_min;};
    Point3 max() const {return m_max;};

    bool hit(const Ray& r, float t0, float t1) const {
        return true;
    }

public:
    Point3 m_min; //close corner of bounding box
    Point3 m_max; //far corner of bounding box
};


#endif //AABB_H