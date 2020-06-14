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

class Translate : public Hitable {
public:
    Translate(std::shared_ptr<Hitable> h, const Vec3& displacement) : m_ptr(h), m_displacement(displacement) {};
    virtual bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const
    {
        Ray t_ray(r.origin() - m_displacement, r.direction(), r.time()); //move object by m_displacement is same as moving ray by -m_displacement
        if(m_ptr->hit(t_ray, tMin, tMax, rec)) {
            rec.p += m_displacement;
            rec.set_face_normal(t_ray, rec.normal); //is this line necessary???  Calling the hit function above should already call this function
        }else{
            return false;
        }
    }
    virtual bool bounding_box(float t0, float t1, AABB &output_box) const
    {
        if(m_ptr->bounding_box(t0, t1, output_box)) {
            output_box = AABB(output_box.min() + m_displacement, output_box.max() + m_displacement);
            return true;
        }else{
            return false;
        }
    }
private:
    std::shared_ptr<Hitable> m_ptr;
    Vec3 m_displacement;
};

class RotateY : public Hitable {
public:
    RotateY(std::shared_ptr<Hitable> p, float angle_d);
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& output_box) const{
        output_box = m_bbox;
        return m_hasBox;
    }
private:
    std::shared_ptr<Hitable> m_ptr;
    float m_sinTheta;
    float m_cosTheta;
    bool m_hasBox;
    AABB m_bbox;
};

RotateY::RotateY(std::shared_ptr<Hitable> p , float angle_d) : m_ptr(p) {
    float radians = degrees_to_radians(angle_d);
    m_sinTheta = sin(radians);
    m_cosTheta = cos(radians);
    m_hasBox = m_ptr->bounding_box(0.0f, 1.0f, m_bbox); //fill in m_bbox max and min

    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    //loop through all eight vertices of cube
    //and clamp bounding box in all three dimensions
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            for(int k = 0; k < 2; ++k) {
                //get coordinates of one vertex of cube
                float x = i * m_bbox.max().x() + (1 - i) * m_bbox.min().x();
                float y = j * m_bbox.max().y() + (1 - j) * m_bbox.min().y();
                float z = k * m_bbox.max().z() + (1 - k) * m_bbox.min().z();

                float newx = m_cosTheta * x + m_sinTheta * z;
                float newz = -m_sinTheta * x + m_cosTheta * z;

                Vec3 tester(newx, y, newz);

                //clamp in all dimensions
                for(int c = 0; c < 3; ++c) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    m_bbox = AABB(min, max);
}

bool RotateY::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    Point3 origin = r.origin();
    Vec3 direction = r.direction();

    //rotate x and z coordinates of origin and direction
    origin[0] = m_cosTheta * r.origin()[0] - m_sinTheta * r.origin()[2];
    origin[2] = m_sinTheta * r.origin()[0] + m_cosTheta * r.origin()[2];

    direction[0] = m_cosTheta * r.direction()[0] - m_sinTheta * r.direction()[2];
    direction[2] = m_sinTheta * r.direction()[0] + m_cosTheta * r.direction()[2];

    Ray rotated_r(origin, direction, r.time());

    //check if rotated ray hits Hitable
    if(!m_ptr->hit(rotated_r, tMin, tMax, rec)){
        return false;
    }else{
        //rotate values in HitRecord (opposite rotation from rotating ray)
        Point3 p = rec.p;
        Vec3 normal = rec.normal;

        p[0] = m_cosTheta * rec.p[0] + m_sinTheta * rec.p[2];
        p[2] = -m_sinTheta * rec.p[0] + m_cosTheta * rec.p[2];

        normal[0] = m_cosTheta * rec.normal[0] + m_sinTheta * rec.normal[2];
        normal[2] = -m_sinTheta * rec.normal[0] + m_cosTheta * rec.normal[2];

        rec.p = p;
        rec.set_face_normal(rotated_r, normal);

        return true;
    }

}

#endif //HITABLE_H