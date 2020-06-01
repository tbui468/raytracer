#ifndef SPHERE_H
#define SPHERE_H

#include "Hitable.h"

class Sphere : public Hitable {
public:
    Sphere() {};
    Sphere(Vec3 center, float radius): m_center(center), m_radius(radius) {};
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
private:
    Vec3 m_center;
    float m_radius;
};

//returns if ray hit sphere.  Saves data in Hit_Record struct if hit
bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    Vec3 AMinusC = r.origin() - m_center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0f * dot(r.direction(), AMinusC);
    float c = dot(AMinusC, AMinusC) - m_radius * m_radius;
    float discriminant = b * b - 4.0f * a * c;
    if(discriminant > 0.0f) {
        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        if( t > tMin && t < tMax) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.normal = (rec.p - m_center);
            rec.normal.make_unit_vector();
            return true;
        }
        t = (-b + sqrt(discriminant)) / (2.0f * a);
        if(t > tMin && t < tMax) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.normal = (rec.p - m_center);
            rec.normal.make_unit_vector();
            return true;
        }
    }
    return false;
}


#endif //SPHERE_H