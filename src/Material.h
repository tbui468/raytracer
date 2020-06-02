#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hitable.h"


Vec3 random_sphere_unit_vector();

class Material
{
public:
    //scatter function determines how ray interacts with material (eg, specular or diffuse reflection)
    virtual bool scatter(const Ray &r, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Vec3& a) : m_albedo(a) {};
    bool scatter(const Ray &r, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
        Vec3 target = rec.p + rec.normal + random_sphere_unit_vector();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = m_albedo;
        return true;
    }
    Vec3 m_albedo;
};

class Metal : public Material
{
public:
    Metal(const Vec3& a, float f) : m_albedo(a) {if(f < 1.0f) m_fuzz = f; else m_fuzz = 1.0f;};
    bool scatter(const Ray &r, const HitRecord &rec, Vec3 &attenuation, Ray &scattered) const override {
        Vec3 target = unit_vector(reflect(r.direction(), rec.normal));
        scattered = Ray(rec.p, target + m_fuzz * random_sphere_unit_vector());
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
    bool scatter(const Ray& r, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const override{
        Vec3 outward_normal; 
        Vec3 reflected = reflect(r.direction(), rec.normal);
        float n1_over_n2;
        attenuation = Vec3(1.0f, 1.0f, 1.0f);
        Vec3 refracted;
        float reflect_prob;
        float cosine;

        //reverse normal/refractive index ratio depending on 
        //whether ray is leaving or entering dielectric material
        if(dot(r.direction(), rec.normal) > 0.0f) { //if ray is leaving material into air
            outward_normal = -rec.normal;
            n1_over_n2 = m_refractiveIndex;
            cosine = dot(r.direction(), rec.normal) / r.direction().length();
            cosine = sqrt(1.0f - m_refractiveIndex * m_refractiveIndex * (1.0f - cosine * cosine));
        }else{ //ray is entering material from air
            outward_normal = rec.normal;
            n1_over_n2 = 1.0f / m_refractiveIndex;
            cosine = -dot(r.direction(), rec.normal) / r.direction().length();
        }

        //calculate reflection probabililty using schlick approximation
        if(refract(r.direction(), outward_normal, n1_over_n2, refracted)) {
            reflect_prob = schlick(cosine, m_refractiveIndex);
        }else{
            reflect_prob = 1.0f;
        }

        //reflect or refract based on probability
        if(float(rand())/RAND_MAX < reflect_prob) {
            scattered = Ray(rec.p, reflected);
        }else{
            scattered = Ray(rec.p, refracted);
        }

        return true;
    }
    float m_refractiveIndex;
};

#endif //MATERIAL_H