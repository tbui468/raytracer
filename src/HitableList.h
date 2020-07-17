#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "Hitable.h"

#include <memory>
#include <vector>

class HitableList : public Hitable {
public:
    HitableList() {};
    HitableList(std::shared_ptr<Hitable> object) {m_objects.push_back(object);};
    void add(std::shared_ptr<Hitable> object) {m_objects.push_back(object);};
    void clear() {m_objects.clear();};
    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    bool bounding_box(float t0, float t1, AABB& output_box) const;
    float pdf_value(const Point3& origin, const Vec3& direction) const;
    Vec3 random(const Point3& origin) const;
public:
    std::vector<std::shared_ptr<Hitable>> m_objects;
};

//loop through list of Hitables and returns true if anything is hit
//return HitRecord with closest Hitable that intersects Ray
bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;
    float closestT = tMax;
    for(const std::shared_ptr<Hitable>& object : m_objects) {
        if(object->hit(r, tMin, closestT, tempRec)) {
            closestT = tempRec.t;
            hitAnything = true;
            rec = tempRec;
        }
    }
    return hitAnything;
}

//
bool HitableList::bounding_box(float t0, float t1, AABB &output_box) const
{
    if(m_objects.empty()) return false;

    AABB temp_box;
    bool first_box = true;

    //for each Hitable
    for(const std::shared_ptr<Hitable>& object : m_objects) {
        if(!object->bounding_box(t0, t1, temp_box)) return false; //return false if hitable is not boundable by AABB (not possible now with only spheres)
        if(first_box) { //set output to temp_box if no previous box
            output_box = temp_box;
            first_box = false;
        }else{ //set output to box enclosing temp_box and the previously constructed box
            output_box = surrounding_box(output_box, temp_box);
        }
    }
    return true;
}

float HitableList::pdf_value(const Point3 &origin, const Vec3 &direction) const {
    size_t size = m_objects.size();
    float sum = 0.0f;
    for(auto obj: m_objects) {
        sum += obj->pdf_value(origin, direction);
    }
    return sum / size;
}

Vec3 HitableList::random(const Point3 &origin) const {
    auto obj = m_objects.at(randi(0, m_objects.size()));
    return obj->random(origin);
}

#endif //HITABLELIST_H