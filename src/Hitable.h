#ifndef HITABLE_H
#define HITABLE_H

#include "Ray.h" 

class Material;

struct HitRecord {
    float t; //paramter t of where ray hit hitable object
    Vec3 p; //coordinates of location hit
    Vec3 normal; //normal of location hit
    Material* material_ptr; //this ptr will be set to the material of any sphere it hits
};

class Hitable {
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const = 0; //pure virtual function
};

#endif //HITABLE_H