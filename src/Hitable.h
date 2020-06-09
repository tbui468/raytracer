#ifndef HITABLE_H
#define HITABLE_H


class Material;

struct HitRecord {
    float t; //paramter t of where ray hit hitable object
    Point3 p; //coordinates of location hit
    Vec3 normal; //normal of location hit
    bool front_face; //front face of Hitable???
    std::shared_ptr<Material> material_ptr;
    float u; //texture coordinate
    float v; //texture coordinate

    //sets normal and front_face of Hitable
    inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0.0f;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hitable {
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0; //pure virtual function
    virtual bool bounding_box(float t0, float t1, AABB& output_box) const = 0; //t0 and t1 are for moving spheres (bounding box encloses all locations)
};

#endif //HITABLE_H