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


#endif //HITABLELIST_H