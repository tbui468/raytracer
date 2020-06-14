#ifndef AABB_H
#define AABB_H


class AABB {
public:
    AABB() {};
    AABB(const Point3& min, const Point3& max): m_min(min), m_max(max) {};
    Point3 min() const {return m_min;};
    Point3 max() const {return m_max;};

    //determine if ray hits box
    bool hit(const Ray& r, float tmin, float tmax) const {
        
        //loop through x, y and z dimensions of axis-aligned bounding box
        for(int i = 0; i < 3; ++i) {
            //calculate t0 and t1 where t0 < t1
            float t0 = fmin((m_min[i] - r.origin()[i]) / r.direction()[i],
                            (m_max[i] - r.origin()[i]) / r.direction()[i]);
            float t1 = fmax((m_min[i] - r.origin()[i]) / r.direction()[i],
                            (m_max[i] - r.origin()[i]) / r.direction()[i]);

            //find new tmin/tmax by finding overlap between current and previous t values (function input parameters for i = 0)
            tmin = fmax(t0, tmin);
            tmax = fmin(t1, tmax);
            //return false if no overlap
            if(tmax <= tmin)
                return false;
        }
        /*
        //Alternative optimized hit function
        for(int i = 0; i < 3; ++i) {
            float invD = 1.0f / r.direction()[i];
            float t0 = (m_min[i] - r.origin()[i]) * invD;
            float t1 = (m_max[i] - r.origin()[i]) * invD;
            if(invD < 0.0f)
                std::swap(t0, t1);
            tmin = t0 < tmin ? tmin : t0;
            tmax = t1 > tmax ? tmax : t1;
            if(tmax <= tmin)
                return false;
        } */
        
        return true;
    }

private:
    Point3 m_min; //close corner of bounding box
    Point3 m_max; //far corner of bounding box
};

AABB surrounding_box(const AABB& b1, const AABB& b2) {
    Point3 min(fmin(b1.min().x(), b2.min().x()),
                fmin(b1.min().y(), b2.min().y()),
                fmin(b1.min().z(), b2.min().z()));

    Point3 max(fmax(b1.max().x(), b2.max().x()),
                fmax(b1.max().y(), b2.max().y()),
                fmax(b1.max().z(), b2.max().z()));

    return AABB(min, max);
}


#endif //AABB_H