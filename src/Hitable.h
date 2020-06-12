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

//doesnt do anything now bc we have same material on both sides of wall
class FlipFace : public Hitable
{
public:
    FlipFace(std::shared_ptr<Hitable> p) : m_ptr(p){};
    virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const
    {
        if (!m_ptr->hit(r, tMin, tMax, rec))
        {
            return false;
        }
        else
        {
            rec.front_face = !rec.front_face;
            return true;
        }
    }
    virtual bool bounding_box(float t0, float t1, AABB &output_box) const
    {
        return m_ptr->bounding_box(t0, t1, output_box);
    }

private:
    std::shared_ptr<Hitable> m_ptr;
};

#endif //HITABLE_H