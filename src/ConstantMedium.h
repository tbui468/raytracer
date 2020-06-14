#ifndef CONSTANTMEDIUM_H
#define CONSTANTMEDIUM_H

class ConstantMedium : public Hitable {
public:
    ConstantMedium(std::shared_ptr<Hitable> boundry, float density, std::shared_ptr<Texture> tex)
        : m_boundry(boundry), m_negInvDensity(-1.0f / density) {
            m_phaseFunction = std::make_shared<Isotropic>(tex);
        }
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const;

    virtual bool bounding_box(float t0, float t1, AABB& output_box) const {
        return m_boundry->bounding_box(t0, t1, output_box);
    }
public:
    std::shared_ptr<Hitable> m_boundry;
    std::shared_ptr<Material> m_phaseFunction;
    float m_negInvDensity;
};

bool ConstantMedium::hit(const Ray& r, float tMin, float tMax, HitRecord& rec) const {
    const bool enableDebug = false;
    const bool debugging = enableDebug && randf() < 0.00001f;

    HitRecord rec1;
    HitRecord rec2;

    //check if hit occurs in either direction (in case ray starts inside medium)
    if(!m_boundry->hit(r, -infinity, infinity, rec1))
        return false;

    //check if hit occurs in positive direction of rec1.t hit
    if(!m_boundry->hit(r, rec1.t + 0.0001f, infinity, rec2))
        return false;

    if(debugging) std::cerr << "\nt0" << rec1.t << ", t1=" << rec2.t<< '\n';

    //find overlap between intervals (rec1.t, rec2.t) and (tMin, tMax)
    if(rec1.t < tMin) rec1.t = tMin;
    if(rec2.t > tMax) rec2.t = tMax;

    if(rec1.t > rec2.t)
        return false;

    //???
    if(rec1.t < 0.0f)
        rec1.t = 0.0f;

    const float ray_length = r.direction().length();
    const float distance_inside_boundry = (rec2.t - rec1.t) * ray_length;
    const float hit_distance = m_negInvDensity * log(randf());

    if(hit_distance > distance_inside_boundry)
        return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.point_at_parameter(rec.t);

    if(debugging) {
        std::cerr << "hit_distance = " << hit_distance << '\n'
                  << "rec.t = " << rec.t << '\n'
                  << "rec.p = " << rec.p << '\n';
    }

    rec.normal = Vec3(1.0f, 0.0f, 0.0f); //arbitrary
    rec.front_face = true; //arbitrary
    rec.material_ptr = m_phaseFunction;

    return true;
}


#endif //CONSTANTMEDIUM_H