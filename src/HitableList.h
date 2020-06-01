#ifndef HITABLELIST_H
#define HITABLELIST_H

#include "Hitable.h"

class HitableList : public Hitable {
public:
    HitableList() {};
    HitableList(Hitable** l, int count): m_list(l), m_count(count) {};
    bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    Hitable** m_list;
    int m_count;
};

//loop through list of Hitables and returns true if anything is hit
//return HitRecord with closest Hitable that intersects Ray
bool HitableList::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    HitRecord tempRec;
    bool hitAnything = false;
    float closestT = tMax;
    for(int i = 0; i < m_count; ++i) {
        if(m_list[i]->hit(r, tMin, closestT, tempRec)){
            closestT = tempRec.t;
            hitAnything = true;
            rec = tempRec;
        }
    }
    return hitAnything;
}


#endif //HITABLELIST_H