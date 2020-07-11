#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hitable.h"
#include "Texture.h"
#include "ONB.h"


class Material
{
public:
    //scatter function determines how ray interacts with material (eg, specular or diffuse reflection)
    virtual bool scatter(const Ray &r, const HitRecord &rec, Color &albedo, Ray &scattered, float& pdf) const {
        return false;
    }

    virtual float scattering_pdf(const Ray& r, const HitRecord& rec, const Ray& scattered) const {
        return 0.0f;
    }

    virtual Color emitted(float u, float v, const Point3& p) const {
        return Color(0.0f, 0.0f, 0.0f); //default materials emits black (no light)
    }
};

class Lambertian : public Material
{
public:
    Lambertian(std::shared_ptr<Texture> a) : m_albedo(a) {};
    bool scatter(const Ray &r, const HitRecord &rec, Vec3 &albedo, Ray &scattered, float& pdf) const override {
        //this is where we choose a random sample
        //Vec3 direction = random_in_hemisphere(rec.normal); 
        ONB onb;
        onb.build_from_w(rec.normal);
        Vec3 direction = onb.local(random_cosine_direction()); //chosen from hemisphere
        scattered = Ray(rec.p, unit_vector(direction), r.time());  
        albedo = m_albedo->value(rec.u, rec.v, rec.p);
        //the distribution of the random sample
        //pdf = 0.5 / PI; //this is the pdf of the sample
        pdf = dot(onb.w(), scattered.direction()) / PI; //cos(theta) / pi is the pdf of lambertian scattering
        return true;
    }

    //s(direction), which is cosine/PI for Lambertian materials
    //the scattering_pdf does NOT change based on how we sample
    float scattering_pdf(const Ray& r, const HitRecord& rec, const Ray& scattered) const {
        float cosine = dot(rec.normal, unit_vector(scattered.direction()));
        return cosine < 0.0f ? 0.0 : cosine / PI; 
    }
    std::shared_ptr<Texture> m_albedo;
};

class Metal : public Material
{
public:
    Metal(std::shared_ptr<Texture> a, float f) : m_albedo(a) {if(f < 1.0f) m_fuzz = f; else m_fuzz = 1.0f;};
    bool scatter(const Ray &r, const HitRecord &rec, Vec3 &attenuation, Ray &scattered, float& pdf) const override {
        Vec3 target = unit_vector(reflect(r.direction(), rec.normal));
        scattered = Ray(rec.p, target + m_fuzz * random_in_unit_sphere(), r.time());
        //attenuation = m_albedo;
        attenuation = m_albedo->value(rec.u, rec.v, rec.p);
        return dot(scattered.direction(), rec.normal) > 0.0f; //only return true if scatter direction is away from sphere
    }
    std::shared_ptr<Texture> m_albedo;
    float m_fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(float ri) : m_refractiveIndex(ri) {};
    bool scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered, float& pdf) const override{
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
    virtual bool scatter(const Ray& r, const HitRecord& rec, Color& attenuation, Ray& scattered, float& pdf) const {
        return false; //does not scatter light
    }

    virtual Color emitted(float u, float v, const Point3& p) const {
        return m_emit->value(u, v, p); //returns color of texture
    }
public:
    std::shared_ptr<Texture> m_emit;
};

class Isotropic : public Material {
public:
    Isotropic(std::shared_ptr<Texture> tex) : m_albedo(tex) {};
    virtual bool scatter(const Ray &r, const HitRecord &rec, Color &attenuation, Ray &scattered, float& pdf) const {
        scattered = Ray(rec.p, random_in_unit_sphere(), r.time());
        attenuation = m_albedo->value(rec.u, rec.v, rec.p);
        return true;
    }
public:
    std::shared_ptr<Texture> m_albedo;
};

#endif //MATERIAL_H