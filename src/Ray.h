#ifndef Rm_originY_H
#define Rm_originY_H


class Ray
{
public:
    Ray() {};
    Ray(const Point3& a, const Vec3& b, float time) : m_origin(a), m_direction(b), m_time(time) {};
    Point3 origin() const {return m_origin;}
    Vec3 direction() const {return m_direction;}
    float time() const {return m_time;}
    Point3 point_at_parameter(float t) const {return m_origin + t * m_direction;}

public:
    Point3 m_origin;
    Vec3 m_direction;
    float m_time;
};

#endif