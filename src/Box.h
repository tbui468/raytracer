#ifndef BOX_H
#define BOX_H

class Box : public Hitable {
public:
    Box() {};
    Box(const Point3& min, const Point3& max, std::shared_ptr<Material> m);
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& output_box) const;
public:
    Point3 m_min;
    Point3 m_max;
    HitableList m_sides;
};

Box::Box(const Point3& min, const Point3& max, std::shared_ptr<Material> m) : m_min(min), m_max(max) {
    //front and back (flipped)
    m_sides.add(std::make_shared<XYRect>(min.x(), max.x(), min.y(), max.y(), max.z(), m));
    m_sides.add(std::make_shared<FlipFace>(std::make_shared<XYRect>(min.x(), max.x(), min.y(), max.y(), min.z(), m)));

    //top and bottom (flipped)
    m_sides.add(std::make_shared<XZRect>(min.x(), max.x(), min.z(), max.z(), max.y(), m));
    m_sides.add(std::make_shared<FlipFace>(std::make_shared<XZRect>(min.x(), max.x(), min.z(), max.z(), min.y(), m)));

    //right and left (flipped)
    m_sides.add(std::make_shared<YZRect>(min.y(), max.y(), min.z(), max.z(), max.x(), m));
    m_sides.add(std::make_shared<FlipFace>(std::make_shared<YZRect>(min.y(), max.y(), min.z(), max.z(), min.x(), m)));
}


bool Box::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    return m_sides.hit(r, tMin, tMax, rec);
}
bool Box::bounding_box(float t0, float t1, AABB& output_box) const {
    output_box = AABB(m_min, m_max);
    return true;
}



#endif //BOX_H