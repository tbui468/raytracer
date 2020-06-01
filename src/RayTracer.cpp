#include <iostream>
#include <fstream>

#include "Ray.h" //Ray.h includes Vec3.h

Vec3 color(const Ray& r) {
    Vec3 unit_dir = unit_vector(r.direction());
    float t = 0.5f * (unit_dir.y() + 1.0f);
    return ((1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.3f, 0.5f, 1.0f));
}

int main()
{
    std::ofstream imageFile;
    imageFile.open("image.ppm");

    int nx = 200;
    int ny = 100;

    //P3: ASCII ppm file, width , height, 255: max value
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";

    Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    for(int j = ny - 1; j >= 0; --j) {
        for(int i = 0; i < nx; ++i) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            Ray r(origin, lower_left_corner + horizontal * u + vertical * v);
            Vec3 c = color(r);

            int ir = int(255.99f * c[0]);
            int ig = int(255.99f * c[1]);
            int ib = int(255.99f * c[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    //test vec3
    Vec3 v1(1.0f, 2.0f, 4.0f);
    Vec3 v2(2.0f, 7.0f, -5.0f);

    v1.make_unit_vector();

    std::cout << v1;

    imageFile.close();
    return 0;
}