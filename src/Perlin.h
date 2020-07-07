#ifndef PERLIN_H
#define PERLIN_H

inline float trilinear_interp(float c[2][2][2], float u, float v, float w) {
    float accum = 0.0f;

    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 2; ++j)
            for(int k = 0; k < 2; ++k)
                accum += (i*u + (1-i)*(1-u))*
                         (j*v + (1-j)*(1-v))*
                         (k*w + (1-k)*(1-w))*c[i][j][k];


    return accum;
}

//does a bunch of computations for a hash
//not important to understand now
class Perlin {
public:
    Perlin(){
        m_randVec = new Vec3[POINT_COUNT];

        for(int i = 0; i < POINT_COUNT; ++i) {
            m_randVec[i] = random_unit_vector();
        }

        m_permX = perlin_generate_perm();
        m_permY = perlin_generate_perm();
        m_permZ = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] m_randVec;
        delete[] m_permX;
        delete[] m_permY;
        delete[] m_permZ;
    }

    float noise(const Point3& p) const {
        //smoothstep parameter (weight) 
        //each paramter is 0 to .9999
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        u = u * u * (3-2*u);
        v = v * v * (3-2*v);
        w = w * w * (3-2*w);

        //get lower corner of cube
        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        Vec3 c[2][2][2]; //create a 2x2x2 array of Vec3

        for(int di = 0; di < 2; ++di)
            for(int dj = 0; dj < 2; ++dj)
                for(int dk = 0; dk < 2; ++dk) 
                    c[di][dj][dk] = m_randVec[
                            m_permX[(i+di) & 255] ^ // ^ is bitwise xor
                            m_permY[(j+dj) & 255] ^ //i still don't get this
                            m_permZ[(k+dk) & 255]
                    ];

        return perlin_interp(c, u, v, w);
    }

    float turb(const Point3& p, int depth = 7) const {
        float accum = 0.0f;
        Point3 temp_p = p;
        float weight = 1.0f;

        for(int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5f;
            temp_p *= 2.0f;
        }

        return fabs(accum);
    }
private:
    static const int POINT_COUNT = 256;
    Vec3* m_randVec;
    int* m_permX;
    int* m_permY;
    int* m_permZ;

    static int* perlin_generate_perm() {
        int* p = new int[POINT_COUNT];

        for(int i = 0; i < POINT_COUNT; i++) {
            p[i] = i;
        }

        permute(p, POINT_COUNT);

        return p;
    }

    static void permute(int* p, int count) {
        for (int i = count - 1; i > 0; --i)
        {
            int target = randi(0, count);
            std::swap(p[i], p[target]);
        }
    }

    inline float perlin_interp(Vec3 c[2][2][2], float u, float v, float w) const
    {
        //uu, vv, and ww are the paramters (weights) 
        //aren't u, v and w already smoothed in noise() function????
        //applying smoothstep twice or is it a bug???
        float uu = u * u * (3.0f - 2.0f * u);
        float vv = v * v * (3.0f - 2.0f * v);
        float ww = w * w * (3.0f - 2.0f * w);
        float accum = 0.0f;

        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                for (int k = 0; k < 2; ++k) {
                    Vec3 weight_v(u - i, v - j, w - k);
                    accum += (i*uu + (1.0f - i)*(1.0f-uu))
                           * (j*vv + (1.0f - j)*(1.0f-vv))
                           * (k*ww + (1.0f - k)*(1.0f-ww))
                           * dot(c[i][j][k], weight_v);
                }
        return accum;
    }
};

#endif //PERLIN_H