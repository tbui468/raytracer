#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class Vec3
{
public:
    Vec3(): e{0.0f, 0.0f, 0.0f} {};
    Vec3(float e0, float e1, float e2) : e{ e0, e1, e2} {};

    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); } //returns negative Vec3 copy, doesn't modify data
    float operator[](int i) const { return e[i]; }               //returns copy, doesn't modify data
    float &operator[](int i) { return e[i]; }                    //returns reference that can modify data

    Vec3& operator+=(const Vec3& other) {
        e[0] += other.e[0];
        e[1] += other.e[1];
        e[2] += other.e[2];
        return *this;
    }

    Vec3& operator*=(const float scalar) {
        e[0] *= scalar;
        e[1] *= scalar;
        e[2] *= scalar;
        return *this;
    }

    Vec3& operator/=(const float scalar) {
        return *this *= (1.0f / scalar);
    }

    float length() const
    {
        return sqrt(squared_length());
    }

    float squared_length() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline static Vec3 random() {
        return Vec3(randf(), randf(), randf());
    }

    inline static Vec3 random(float min, float max) {
        return Vec3(randf(min, max), randf(min, max), randf(min, max));
    }

public:
    float e[3];
};

//type alias for color
using Color = Vec3;
using Point3 = Vec3;


#endif //VEC3_H


//uitility functions for Vec3

//puts stream elements into the destination Vec3
inline std::istream &operator>>(std::istream &is, Vec3 &v)
{
    return is >> v.e[0] >> v.e[1] >> v.e[2];
}

//outputs Vec3 elements
inline std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    return os << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

/*
inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}*/

inline Vec3 operator*(const Vec3 &v, float s)
{
    return Vec3(s * v.e[0], s * v.e[1], s * v.e[2]);
}

inline Vec3 operator*(float s, const Vec3 &v)
{
    return v * s;
}

inline Vec3 operator/(const Vec3 &v, float s)
{
    return (1.0f / s) * v;
}

inline float dot(const Vec3 &v1, const Vec3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    Vec3 unitNormal = n / n.length();
    return v - 2.0f * dot(v, unitNormal) * unitNormal;
}


Vec3 refract(const Vec3& uv, const Vec3& n, float etai_over_etat) {
    float cos_theta = dot(-uv, n);
    Vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_perp = -sqrt(1.0f - r_out_parallel.squared_length()) * n;
    return r_out_parallel + r_out_perp;
}

inline float schlick(float cosine, float ref_index) {
    float r0 = (1.0f - ref_index) / (1.0f + ref_index);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5);
}


inline Vec3 unit_vector(Vec3 v)
{
    return v / v.length();
}

//Lambertian diffuse hack (not accurate)
//random point INSIDE unit sphere
Vec3 random_in_unit_sphere() {
    Vec3 v;
    do{
        v = Vec3::random(-1.0f, 1.0f);
    }while(v.squared_length() >= 1.0f);
    return v;
}

//correct Lambertian diffuse
//random point ON unit sphere, normalized
Vec3 random_unit_vector() {
    float angle = randf(0.0f, 2.0f * PI);
    float z = randf(-1.0f, 1.0f);
    float radius = sqrt(1.0f - z * z);
    return Vec3(radius * cos(angle), radius * sin(angle), z);
}

//common method before Lambertian diffuse
//uniform distributed in hemisphere
Vec3 random_in_hemisphere(const Vec3& normal) {
    Vec3 in_unit_sphere = random_in_unit_sphere();
    if(dot(in_unit_sphere, normal) > 0.0f) 
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vec3 random_cosine_direction() {
    float r1 = randf();
    float r2 = randf();
    float z = sqrt(1 - r2);
    float phi = 2 * PI * r1;
    float x = cos(phi) * sqrt(r2);
    float y = sin(phi) * sqrt(r2);
    return Vec3(x, y, z);
}