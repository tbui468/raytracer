#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hitable.h"
#include "Texture.h"


class Material
{
public:
    //scatter function determines how ray interacts with material (eg, specular or diffuse reflection)
    virtual bool scatter(const Ray &r, const HitRecord &rec, Color &attenuation, Ray &scattered) const = 0;
    virtual Color emitted(float u, float v, const Point3& p) const {
        return Color(0.0f, 0.0f, 0.0f); //default materials emits black (no light)
    }
};

class Lambertian : public Material
{
public:
    Lambertian(std::shared_ptr<Texture> a) : m_albedo(a) {};
    bool scatter(const Ray &r, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
        scattered = Ray(rec.p, rec.normal + random_unit_vector(), r.time()); 
        attenuation = m_albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
    //Vec3 m_albedo;
    std::shared_ptr<Texture> m_albedo;
};

class Metal : public Material
{
public:
    Metal(const Vec3& a, float f) : m_albedo(a) {if(f < 1.0f) m_fuzz = f; else m_fuzz = 1.0f;};
    bool scatter(const Ray &r, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
        Vec3 target = unit_vector(reflect(r.direction(), rec.normal));
        scattered = Ray(rec.p, target + m_fuzz * random_in_unit_sphere(), r.time());
        attenuation = m_albedo;
        return dot(scattered.direction(), rec.normal) > 0.0f; //only return true if scatter direction is away from sphere
    }
    Vec3 m_albedo;
    float m_fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(float ri) : m_refractiveIndex(ri) {};
    bool scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered) const override{
        attenuation = Color(1.0f, 1.0f, 1.0f); //reflect all
        float etai_over_etat = rec.front_face ? (1.0f / m_refractiveIndex) : (m_refractiveIndex);
        Vec3 unit_direction = unit_vector(r.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
        float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

        //first condition is for total internal reflection
        //schlick approximation is for reflections at shallow angles
        if (etai_over_etat * sin_theta > 1.0f || randf() < schlick(cos_theta, etai_over_etat))
        {
            Vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray(rec.p, reflected, r.time());
            return true;
        }
        else //refract ray
        {
            Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
            scattered = Ray(rec.p, refracted, r.time());
            return true;
        }
    }

public:
    float m_refractiveIndex;
};

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> a) : m_emit(a) {};
    virtual bool scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered) const {
        return false; //does not scatter light
    }

    virtual Color emitted(float u, float v, const Point3& p) const {
        return m_emit->value(u, v, p); //returns color of texture
    }
public:
    std::shared_ptr<Texture> m_emit;
};

#endif //MATERIAL_H