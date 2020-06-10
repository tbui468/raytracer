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
        m_randFloat = new float[POINT_COUNT];

        for(int i = 0; i < POINT_COUNT; ++i) {
            m_randFloat[i] = randf();
        }

        m_permX = perlin_generate_perm();
        m_permY = perlin_generate_perm();
        m_permZ = perlin_generate_perm();
    }

    ~Perlin() {
        delete[] m_randFloat;
        delete[] m_permX;
        delete[] m_permY;
        delete[] m_permZ;
    }

    float noise(const Point3& p) const {
        float u = p.x() - floor(p.x());
        float v = p.y() - floor(p.y());
        float w = p.z() - floor(p.z());
        u = u * u * (3-2*u);
        v = v * v * (3-2*v);
        w = w * w * (3-2*w);

        /*int i = static_cast<int>(4*p.x()) & 255;
        int j = static_cast<int>(4*p.y()) & 255;
        int k = static_cast<int>(4*p.z()) & 255;

        return m_randFloat[m_permX[i] ^ m_permY[j] ^ m_permZ[k]];*/

        int i = floor(p.x());
        int j = floor(p.y());
        int k = floor(p.z());
        float c[2][2][2];

        for(int di = 0; di < 2; ++di)
            for(int dj = 0; dj < 2; ++dj)
                for(int dk = 0; dk < 2; ++dk) 
                    c[di][dj][dk] = m_randFloat[
                            m_permX[(i+di) & 255] ^
                            m_permY[(j+dj) & 255] ^
                            m_permZ[(k+dk) & 255]
                    ];

        return trilinear_interp(c, u, v, w);
    }
private:
    static const int POINT_COUNT = 256;
    float* m_randFloat;
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
        for(int i = count - 1; i > 0; --i) {
            int target = randi(0, i + 1);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }
};


#endif //PERLIN_H