#include <iostream>
#include <fstream>
#include <random>

const double PI = 3.1415926535897;

#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"


Vec3 random_sphere_unit_vector() {
    Vec3 v;
    do{
        v = 2.0 * Vec3(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX) - Vec3(1.0f, 1.0f, 1.0f);
    }while(v.squared_length() >= 1.0f);
    return v;
}

//returns a Vec3 color
//depth is the maximum number of reflections
Vec3 color(const Ray &r, Hitable* world, int depth)
{
    HitRecord rec;
    if(world->hit(r, 0.001f, 100.0f, rec)) {
        Ray scattered;
        Vec3 attenuation;
        if(depth < 50 && rec.material_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * color(scattered, world, depth + 1);
        }else{
            return Vec3(0.0f, 0.0f, 0.0f);
        }
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

    Hitable* list[5];
    list[0] = new Sphere(Vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vec3(0.5f, 0.7f, 1.0f)));
    list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vec3(0.5f, 0.5f, 0.5f)));
    list[2] = new Sphere(Vec3(0.5f, 0.0f, -1.0f), 0.5f, new Metal(Vec3(0.6f, 1.0f, 0.8f), 0.1f));
    list[3] = new Sphere(Vec3(-0.2f, -0.25f, -0.75f), 0.25f, new Lambertian(Vec3(1.0f, 0.5f, 0.7f)));
    list[4] = new Sphere(Vec3(0.0f, 0.0f, -2.0f), 0.5f, new Dielectric(1.5f));
    Hitable* world = new HitableList(list, 5);
    Camera cam(Vec3(-2.0f, 2.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), 40.0f, float(nx)/float(ny));

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
                col += color(r, world, 0); //check all objects
            }
            col /= float(ns);

            int ir = int(255.99f * sqrt(col[0]));
            int ig = int(255.99f * sqrt(col[1]));
            int ib = int(255.99f * sqrt(col[2]));
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();
    return 0;
}