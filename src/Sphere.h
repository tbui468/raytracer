#ifndef SPHERE_H
#define SPHERE_H

#include "Hitable.h"

class Sphere : public Hitable {
public:
    Sphere() {};
    Sphere(Point3 center0, Point3 center1, float time0, float time1, float radius, std::shared_ptr<Material> materialPtr)
            : m_center0(center0), m_center1(center1), m_time0(time0), m_time1(time1), m_radius(radius), m_materialPtr(materialPtr) {};
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& output_box) const;
    Vec3 center(float time) const;
    float radius() const {return m_radius;};
private:
    Point3 m_center0;
    Point3 m_center1;
    float m_radius;
    float m_time0;
    float m_time1;
    std::shared_ptr<Material> m_materialPtr;
};

bool Sphere::bounding_box(float t0, float t1, AABB& output_box) const {
    Point3 origin0 = center(t0);
    AABB box0(origin0 - Point3(radius(), radius(), radius()),
              origin0 + Point3(radius(), radius(), radius()));
    Point3 origin1 = center(t1);
    AABB box1(origin1 - Point3(radius(), radius(), radius()),
              origin1 + Point3(radius(), radius(), radius()));
    if(radius() < 0.0f) {
        std::swap(box0.m_max, box0.m_min);
        std::swap(box1.m_max, box1.m_min);
    }
    output_box = surrounding_box(box0, box1);
    return true;
}


//set texture coordinates (u,v), with u and v normalized to [0,1]
void get_sphere_uv(const Point3& p, float& u, float& v) {
    float phi = atan2(p.z(), p.x());
    float theta = asin(p.y());
    u = 1.0f - (phi + PI) / (2.0f * PI);
    v = (theta + PI / 2.0f) / PI;
}

//returns if ray hit sphere.  Saves data in Hit_Record struct if hit
bool Sphere::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    Vec3 AMinusC = r.origin() - center(r.time());
    float a = dot(r.direction(), r.direction());
    float halfb = dot(r.direction(), AMinusC); //optimize by canceling out 2 coefficient in quadratic
    float c = dot(AMinusC, AMinusC) - m_radius * m_radius;
    float discriminant = halfb * halfb - a * c;
    if(discriminant > 0.0f) {
        float t = (-halfb - sqrt(discriminant)) / a;
        if( t > tMin && t < tMax) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            Vec3 outward_normal = (rec.p - center(r.time())) / m_radius;
            rec.set_face_normal(r, outward_normal);
            rec.material_ptr = m_materialPtr;
            get_sphere_uv(outward_normal, rec.u, rec.v);
            return true;
        }
        t = (-halfb + sqrt(discriminant)) / a;
        if(t > tMin && t < tMax) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            Vec3 outward_normal = (rec.p - center(r.time())) / m_radius;
            rec.set_face_normal(r, outward_normal);
            rec.material_ptr = m_materialPtr;
            get_sphere_uv(outward_normal, rec.u, rec.v);
            return true;
        }
    }
    return false;
}

Vec3 Sphere::center(float time) const {
    return m_center0 + (time - m_time0) / (m_time1 - m_time0) * (m_center1 - m_center0);
}



#endif //SPHERE_H