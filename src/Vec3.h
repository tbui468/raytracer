#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class Vec3
{
public:
    Vec3(){};
    Vec3(float e0, float e1, float e2)
    {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    };

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vec3 &operator+() const { return *this; }              //returns referene, doesn't modify data
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); } //returns negative Vec3 copy, doesn't modify data
    inline float operator[](int i) const { return e[i]; }               //returns copy, doesn't modify data
    inline float &operator[](int i) { return e[i]; }                    //returns reference that can modify data

    //these functions change the Vec3 (but not the rhs Vec3/float)
    inline Vec3 &operator+=(const Vec3 &v);
    inline Vec3 &operator-=(const Vec3 &v);
    inline Vec3 &operator*=(const Vec3 &v);
    inline Vec3 &operator/=(const Vec3 &v);
    inline Vec3 &operator*=(const float t);
    inline Vec3 &operator/=(const float t);

    inline float length() const
    {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
    inline float squared_length() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    inline void make_unit_vector();

    float e[3];
};

//puts stream elements into the destination Vec3
inline std::istream &operator>>(std::istream &is, Vec3 &v)
{
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}

//outputs Vec3 elements
inline std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    os << v.e[0] << " " << v.e[1] << " " << v.e[2];
    return os;
}

inline void Vec3::make_unit_vector()
{
    float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
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

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v, float s)
{
    return Vec3(s * v.e[0], s * v.e[1], s * v.e[2]);
}

inline Vec3 operator*(float s, const Vec3 &v)
{
    return Vec3(s * v.e[0], s * v.e[1], s * v.e[2]);
}

//author uses copy instead of const reference for Vec3(typo or is there something i don't see?)
inline Vec3 operator/(const Vec3 &v, float s)
{
    return Vec3(v.e[0] / s, v.e[1] / s, v.e[2] / s);
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

inline Vec3 &Vec3::operator+=(const Vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline Vec3 &Vec3::operator-=(const Vec3 &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline Vec3 &Vec3::operator*=(const Vec3 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline Vec3 &Vec3::operator/=(const Vec3 &v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline Vec3 &Vec3::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}
inline Vec3 &Vec3::operator/=(const float t)
{
    float k = 1.0f / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline Vec3 unit_vector(Vec3 v)
{
    return v / v.length();
}

#endif //VEC3_H