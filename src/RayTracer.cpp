#include <iostream>
#include <fstream>
#include <random>

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

//returns a Vec3 color
Vec3 color(const Ray &r, Hitable* world)
{
    HitRecord rec;
    if(world->hit(r, 0.0f, 100.0f, rec)) {
        return 0.5f * Vec3(rec.normal.x() + 1.0f, rec.normal.y() + 1.0f, rec.normal.z() + 1.0f);
    }
    else //else returns background color(blue/white gradient)
    {
        Vec3 unit_dir = unit_vector(r.direction());
        float p = 0.5f * (unit_dir.y() + 1.0f);
        return ((1.0f - p) * Vec3(1.0f, 1.0f, 1.0f) + p * Vec3(0.5f, 0.7f, 1.0f));
    }
}

int main()
{
    std::ofstream imageFile;
    imageFile.open("image.ppm");

    int nx = 200;
    int ny = 100;
    int ns = 100; //number of random samples

    //P3: ASCII ppm file, width , height, 255: max value
    imageFile << "P3\n"
              << nx << " " << ny << "\n255\n";

    Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Vec3 origin(0.0f, 0.0f, 0.0f);

    Hitable* list[2];
    list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
    Hitable* world = new HitableList(list, 2);
    Camera cam;

    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            Vec3 col(0.0f, 0.0f, 0.0f);
            for (int k = 0; k < ns; ++k)
            {

                float u = float(i + float(rand())/RAND_MAX) / float(nx);
                float v = float(j + float(rand())/RAND_MAX) / float(ny);
                Ray r = cam.getRay(u, v);
                col += color(r, world);
            }
            col /= float(ns);

            int ir = int(255.99f * col[0]);
            int ig = int(255.99f * col[1]);
            int ib = int(255.99f * col[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();
    return 0;
}