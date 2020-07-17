#ifndef PDF_H
#define PDF_H


class PDF {
public:
    PDF() {}
    virtual float value(const Point3& direction) const = 0;
    virtual Vec3 generate() const = 0; 
};

class CosinePDF : public PDF {
public:
    CosinePDF(const Vec3& w) :uvw(ONB(w)) {}

    //get the pdf(direction) value
    //direction is generated using generate() function
    virtual float value(const Point3& direction) const {
        float cosine = dot(unit_vector(direction), uvw.w());
        return (cosine < 0.0f) ? 0.0f : cosine / PI;
    }
    virtual Vec3 generate() const {
        return uvw.local(random_cosine_direction());
    }
public:
    ONB uvw;
};

class HittablePDF : public PDF {
public:
    HittablePDF(std::shared_ptr<Hitable> ptr, const Point3& origin) : m_ptr(ptr), m_origin(origin) {}
    virtual float value(const Point3& direction) const {
        return m_ptr->pdf_value(m_origin, direction);
    }
    virtual Vec3 generate() const {
        return m_ptr->random(m_origin);
    }
private:
    Point3 m_origin;
    std::shared_ptr<Hitable> m_ptr;
};

class MixturePDF : public PDF {
public:
    MixturePDF(std::shared_ptr<PDF> p0, std::shared_ptr<PDF> p1) {
        m_pdf[0] = p0;
        m_pdf[1] = p1;
    }

    virtual float value(const Point3& direction) const {
        //return the average of the two pdf values
        return 0.5f * m_pdf[0]->value(direction) + 0.5f * m_pdf[1]->value(direction);
    }

    //use pdf0 half of the time, use pdf1 the other half of the time
    virtual Vec3 generate() const {
        if(randf() < 0.5f)
            return m_pdf[0]->generate();
        else
            return m_pdf[1]->generate();
    }

private:
    std::shared_ptr<PDF> m_pdf[2];
};

#endif //PDF_H