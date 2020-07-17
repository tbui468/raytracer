#ifndef MATERIAL_H
#define MATERIAL_H

#include "Hitable.h"
#include "Texture.h"
#include "ONB.h"
#include "PDF.h"


//records how details for scatter
struct ScatterRecord{
    Ray specular_ray;
    bool is_specular;
    Color attenuation;
    std::shared_ptr<PDF> pdf_ptr;
};


class Material
{
public:
    //scatter function determines how ray interacts with material (eg, specular or diffuse reflection)
    virtual bool scatter(const Ray &r_in, const HitRecord &rec, ScatterRecord& srec) const {
        return false;
    }

    virtual float scattering_pdf(const Ray& r, const HitRecord& rec, const Ray& scattered) const {
        return 0.0f;
    }

    //virtual Color emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Point3& p) const {
    virtual Color emitted(const Ray& r_in, const HitRecord& rec) const {
        return Color(0.0f, 0.0f, 0.0f); //default materials emits black (no light)
    }
};

class Lambertian : public Material
{
public:
    Lambertian(std::shared_ptr<Texture> a) : m_albedo(a) {};
    bool scatter(const Ray &r, const HitRecord &rec, ScatterRecord& srec) const override {
        srec.is_specular = false;
        srec.attenuation = m_albedo->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = std::make_shared<CosinePDF>(rec.normal);
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
    bool scatter(const Ray &r, const HitRecord &rec, ScatterRecord& srec) const override {
        Vec3 target = unit_vector(reflect(r.direction(), rec.normal));
        srec.specular_ray = Ray(rec.p, target + m_fuzz * random_in_unit_sphere(), r.time());
        srec.attenuation = m_albedo->value(rec.u, rec.v, rec.p);
        srec.is_specular = true;
        srec.pdf_ptr = nullptr; //we won't use the pdf if specular
        return dot(srec.specular_ray.direction(), rec.normal) > 0.0f; 
    }
    std::shared_ptr<Texture> m_albedo;
    float m_fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(float ri) : m_refractiveIndex(ri) {};
    bool scatter(const Ray& r, const HitRecord& rec, ScatterRecord& srec) const override{
        
        srec.attenuation = Color(1.0f, 1.0f, 1.0f); //reflect all
        srec.is_specular = true; 
        srec.pdf_ptr = nullptr;
        float etai_over_etat = rec.front_face ? (1.0f / m_refractiveIndex) : (m_refractiveIndex);
        Vec3 unit_direction = unit_vector(r.direction());
        float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0f);
        float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

        //first condition is for total internal reflection
        //schlick approximation is for reflections at shallow angles
        if (etai_over_etat * sin_theta > 1.0f || randf() < schlick(cos_theta, etai_over_etat))
        {
            Vec3 reflected = reflect(unit_direction, rec.normal);
            srec.specular_ray = Ray(rec.p, reflected, r.time());
            return true;
        }
        else //refract ray
        {
            Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
            srec.specular_ray = Ray(rec.p, refracted, r.time());
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

    //virtual Color emitted(const Ray& r_in, const HitRecord& rec, float u, float v, const Point3& p) const {
    virtual Color emitted(const Ray& r_in, const HitRecord& rec) const {
        if(rec.front_face)
            return m_emit->value(rec.u, rec.v, rec.p); //returns color of texture
        else 
            return Color(0.0f, 0.0f, 0.0f);
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