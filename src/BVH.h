#ifndef BVH_H
#define BVH_H

#include <algorithm>

class BVHNode : public Hitable {
public:
    BVHNode();
    BVHNode(HitableList& list, float t0, float t1) : BVHNode(list.m_objects, 0, list.m_objects.size(), t0, t1) {};
    BVHNode(std::vector<std::shared_ptr<Hitable>>& objects, size_t start, size_t end, float t0, float t1); 
    virtual bool hit(const Ray& r, float t0, float t1, HitRecord& rec)const;
    virtual bool bounding_box(float t0, float t1, AABB& output_box) const;
public:
    std::shared_ptr<Hitable> m_left;
    std::shared_ptr<Hitable> m_right;
    AABB m_box;
};

//checks if box is hit (each leaf node is a hitable object, such as sphere)
bool BVHNode::hit(const Ray &r, float t0, float t1, HitRecord &rec) const
{
    //check if hit this node box
    if (!m_box.hit(r, t0, t1))
    {
        return false;
    }
    else
    {
        bool hit_left = m_left->hit(r, t0, t1, rec);
        bool hit_right = m_right->hit(r, t0, hit_left ? rec.t : t1, rec); //ignore any hits that are behind Hitable in left branch
        return hit_left || hit_right;
    }
}

bool BVHNode::bounding_box(float t0, float t1, AABB &output_box) const
{
    output_box = m_box;
    return true;
}


/*****************************************/
//Box comparison functions
/*****************************************/
inline bool box_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b, int axis) {
    AABB box_a;
    AABB box_b;

    //first two parameters are zero (we don't care about moving spheres
    //because we are just splitting Hitables into two groups, roughly)
    if(!a->bounding_box(0, 0, box_a) || !b->bounding_box(0,0, box_b)){
        std::cerr << "No bouding box in BHVNode constructor.\n";
    }

    //compare coordinates of min corner
    return box_a.min().e[axis] < box_b.min().e[axis]; 
}

bool box_x_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b) {
    return box_compare(a, b, 1);
}
bool box_z_compare(const std::shared_ptr<Hitable> a, const std::shared_ptr<Hitable> b) {
    return box_compare(a, b, 2);
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Hitable>> &objects, size_t start, size_t end, float t0, float t1)
{
    int axis = randi(0, 3); //random 0, 1, or 2
    auto comparator = (axis == 0) ? box_x_compare :
                      (axis == 1) ? box_y_compare :
                                    box_z_compare;

    size_t object_span = end - start;


    if(object_span == 1) { //duplicate single Hitable into two leaf nodes (for simplcity's sake)
        m_left = m_right = objects[start];
    }else if(object_span == 2) { //split into two leaf nodes
        if(comparator(objects[start], objects[start + 1])) {
            m_left = objects[start];
            m_right = objects[start + 1];
        }else{
            m_left = objects[start + 1];
            m_right = objects[start];
        }
    }else{ //split list
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        //object span will be three or more
        //mid will be 1 or more
        size_t mid = start + object_span / 2;
        m_left = std::make_shared<BVHNode>(objects, start, mid, t0, t1);
        m_right = std::make_shared<BVHNode>(objects, mid, end, t0, t1);
    }

    AABB box_left;
    AABB box_right;

    if(!m_left->bounding_box(t0, t1, box_left) || !m_right->bounding_box(t0, t1, box_right))
        std::cerr << "No bounding box in BVHNode constructor.\n";

    m_box = surrounding_box(box_left, box_right);
}


#endif //BVH_H