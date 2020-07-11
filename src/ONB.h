#ifndef ONB_H
#define ONB_H


class ONB {
public:
    ONB() {}
    Vec3 u() const {return m_axis[0];}
    Vec3 v() const {return m_axis[1];}
    Vec3 w() const {return m_axis[2];}
    Vec3 local(float a, float b, float c) const {
        return a * u() + b * v() + c * w();
    }

    Vec3 local(const Vec3& a) const {
        return a.x() * u() + a.y() * v() + a.z() * w();
    }
    void build_from_w(const Vec3& n); //build a new orthonormal basis from normal vector n
private:
    Vec3 m_axis[3];
};

//build u,v,w orthonormal basis from w (which we will designate as the normal vector n)
void ONB::build_from_w(const Vec3& n) {
    m_axis[2] = unit_vector(n);
    //choose axis a, such that it is not parallel to n (dot product == 1 means parallel)
    Vec3 a;
    Vec3 x_axis(1.0f, 0.0f, 0.0f);
    Vec3 y_axis(0.0f, 1.0f, 0.0f);
    if(fabs(dot(m_axis[2], x_axis)) < .9f)
        a = x_axis;
    else
        a = y_axis;
    m_axis[1] = unit_vector(cross(w(), a));
    m_axis[0] = unit_vector(cross(w(), v()));
}


#endif //ONB_H