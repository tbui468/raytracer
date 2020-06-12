#ifndef AARECT_H
#define AARECT_H

class XYRect : public Hitable
{
public:
    XYRect(){};
    XYRect(float x0, float x1, float y0, float y1, float k, std::shared_ptr<Material> m) : m_x0(x0), m_x1(x1), m_y0(y0), m_y1(y1), m_k(k), m_material(m)
    {
        if (m_x0 > m_x1)
            std::swap(m_x0, m_x1);
        if (m_y0 > m_y1)
            std::swap(m_y0, m_y1);
    };

    bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const
    {

        //find value of t intersection of ray.z() and k
        float t = (m_k - r.origin().z()) / r.direction().z();
        if (t < tMin || tMax < t)
            return false;
        //find x and y of intersection using the above value of t
        float x = r.origin().x() + t * r.direction().x();
        float y = r.origin().y() + t * r.direction().y();
        //check if x0 < x < x1 and y0 < y < y1
        bool x_inside = m_x0 < x && x < m_x1;
        bool y_inside = m_y0 < y && y < m_y1;
        if (!x_inside || !y_inside)
            return false;

        //set hitrecord
        rec.t = t;
        rec.p = r.point_at_parameter(t);
        Vec3 outward_normal = Vec3(0.0f, 0.0f, 1.0f);
        rec.set_face_normal(r, outward_normal); //sets rec.normal and rec.front_face
        rec.material_ptr = m_material;
        rec.u = (x - m_x0) / (m_x1 - m_x0);
        rec.v = (y - m_y0) / (m_y1 - m_y0);
        return true;
    }
    bool bounding_box(float t0, float t1, AABB &output_box) const
    {
        output_box = AABB(Point3(m_x0, m_y0, m_k - 0.0001f), Point3(m_x1, m_y1, m_k + 0.0001f));
        return true;
    }

public:
    std::shared_ptr<Material> m_material;
    float m_x0;
    float m_x1;
    float m_y0;
    float m_y1;
    float m_k; //z value
};


class YZRect : public Hitable
{
public:
    YZRect(){};
    YZRect(float y0, float y1, float z0, float z1, float k, std::shared_ptr<Material> m) : m_y0(y0), m_y1(y1), m_z0(z0), m_z1(z1), m_k(k), m_material(m)
    {
        if (m_y0 > m_y1)
            std::swap(m_y0, m_y1);
        if (m_z0 > m_z1)
            std::swap(m_z0, m_z1);
    };

    bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const
    {

        //find value of t intersection of ray.x() and k
        float t = (m_k - r.origin().x()) / r.direction().x();
        if (t < tMin || tMax < t)
            return false;
        //find y and z of intersection using the above value of t
        float y = r.origin().y() + t * r.direction().y();
        float z = r.origin().z() + t * r.direction().z();
        //check if z0 < z < z1 and y0 < y < y1
        bool y_inside = m_y0 < y && y < m_y1;
        bool z_inside = m_z0 < z && z < m_z1;
        if (!y_inside || !z_inside)
            return false;

        //set hitrecord
        rec.t = t;
        rec.p = r.point_at_parameter(t);
        Vec3 outward_normal = Vec3(1.0f, 0.0f, 0.0f);
        rec.set_face_normal(r, outward_normal); //sets rec.normal and rec.front_face
        rec.material_ptr = m_material;
        rec.u = (y - m_y0) / (m_y1 - m_y0);
        rec.v = (z - m_z0) / (m_z1 - m_z0);
        return true;
    }
    bool bounding_box(float t0, float t1, AABB &output_box) const
    {
        output_box = AABB(Point3(m_k - 0.0001f, m_y0, m_z0), Point3(m_k + 0.0001f, m_y1, m_z1));
        return true;
    }

public:
    std::shared_ptr<Material> m_material;
    float m_y0;
    float m_y1;
    float m_z0;
    float m_z1;
    float m_k; //x value
};


class XZRect : public Hitable
{
public:
    XZRect(){};
    XZRect(float x0, float x1, float z0, float z1, float k, std::shared_ptr<Material> m) : m_x0(x0), m_x1(x1), m_z0(z0), m_z1(z1), m_k(k), m_material(m)
    {
        if (m_z0 > m_z1)
            std::swap(m_z0, m_z1);
        if (m_x0 > m_x1)
            std::swap(m_x0, m_x1);
    };

    bool hit(const Ray &r, float tMin, float tMax, HitRecord &rec) const
    {

        //find value of t intersection of ray.z() and k
        float t = (m_k - r.origin().y()) / r.direction().y();
        if (t < tMin || tMax < t)
            return false;
        //find z and x of intersection using the above value of t
        float z = r.origin().z() + t * r.direction().z();
        float x = r.origin().x() + t * r.direction().x();
        //check if z0 < z < z1 and x0 < x < x1
        bool z_inside = m_z0 < z && z < m_z1;
        bool x_inside = m_x0 < x && x < m_x1;
        if (!z_inside || !x_inside)
            return false;

        //set hitrecord
        rec.t = t;
        rec.p = r.point_at_parameter(t);
        Vec3 outward_normal = Vec3(0.0f, 1.0f, 0.0f);
        rec.set_face_normal(r, outward_normal); //sets rec.normal and rec.front_face
        rec.material_ptr = m_material;
        rec.u = (z - m_z0) / (m_z1 - m_z0);
        rec.v = (x - m_x0) / (m_x1 - m_x0);
        return true;
    }
    bool bounding_box(float t0, float t1, AABB &output_box) const
    {
        output_box = AABB(Point3(m_x0, m_k - 0.0001f, m_z0), Point3(m_x1, m_k + 0.0001f, m_z1));
        return true;
    }

public:
    std::shared_ptr<Material> m_material;
    float m_z0;
    float m_z1;
    float m_x0;
    float m_x1;
    float m_k; //y value
};

#endif //AARECT_H