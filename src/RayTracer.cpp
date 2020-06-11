#include <iostream>
#include <fstream>


#include "RayTracer.h" //common headers (including stb_image)
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "Color.h"
#include "BVH.h"




//returns a Vec3 color
//depth is the maximum number of reflections
Vec3 color(const Ray &r, const Hitable& world, int depth)
{
    HitRecord rec;
    if(world.hit(r, 0.001f, infinity, rec)) { //check for any hits in world.  If so, fills in HitRecord rec
        Ray scattered;
        Vec3 attenuation;
        if(depth >= 0 && rec.material_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation * color(scattered, world, depth - 1); //recursively checks world again using new scattered light ray
        }else{
            return Vec3(0.0f, 0.0f, 0.0f); //returns black if scatter depth is < 0 or if light is scattered to inside of metal
        }
    }
    else //else returns background color
    {
        Vec3 unit_dir = unit_vector(r.direction());
        float p = 0.5f * (unit_dir.y() + 1.0f);
        return ((1.0f - p) * Vec3(1.0f, 1.0f, 1.0f) + p * Vec3(.5f, 0.7f, 1.0f));
    }
}

int main()
{
    std::ofstream imageFile;
    imageFile.open("image.ppm");

    constexpr int nx = 200;
    constexpr int ny = 200;
    constexpr int ns = 50; //number of samples per pixel
    constexpr int max_depth = 50; // maximum ray reflections

    //P3: ASCII ppm file, width , height, 255: max value
    imageFile << "P3\n"
              << nx << " " << ny << "\n255\n";

    Point3 lower_left_corner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Point3 origin(0.0f, 0.0f, 0.0f);

    float timeStart = 0.0f;
    float timeEnd = 1.0f;

    std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(
              std::make_shared<SolidColor>(1.0f, 1.0f, 1.0f), 
              std::make_shared<SolidColor>(0.2f, 0.2f, 0.2f));

    std::shared_ptr<ImageTexture> earthTexture = std::make_shared<ImageTexture>("../../../src/earthmap.jpg");

    HitableList world;

    world.add(std::make_shared<Sphere>(Vec3(-0.7f, 0.5f, -2.0f), Vec3(-0.7f, 0.5f, -2.0f), timeStart, timeEnd, 
              1.0f, std::make_shared<Lambertian>(std::make_shared<NoiseTexture>(2.0f))));

    //checker world
    world.add(std::make_shared<Sphere>(Vec3(0.0f, -200.5f, -1.0f), Vec3(0.0f, -200.5f, -1.0f), timeStart, timeEnd, 200.0f, std::make_shared<Lambertian>(checker)));/*
    world.add(std::make_shared<Sphere>(Vec3(-1.0f, 0.5f, -1.0f), Vec3(-1.0f, 0.2f, -1.0f), timeStart, timeEnd, 
              0.5f, std::make_shared<Lambertian>(std::make_shared<SolidColor>(0.5f, 0.7f, 1.0f))));
    world.add(std::make_shared<Sphere>(Vec3(1.5f, 0.5f, -1.25f), Vec3(1.5f, 0.5f, -1.25f), timeStart, timeEnd, 1.0f, std::make_shared<Metal>(Vec3(1.0f, 0.5f, 0.7f), 0.01f)));
    world.add(std::make_shared<Sphere>(Vec3(1.0f, 0.0f, -2.0f), Vec3(1.0f, 0.0f, -2.0f), timeStart, timeEnd, 
              0.25f, std::make_shared<Lambertian>(std::make_shared<SolidColor>(0.5f, 0.6f, 0.5f))));
    world.add(std::make_shared<Sphere>(Vec3(0.3f, -0.25f, -0.5f), Vec3(0.3f, -0.25f, -0.5f), timeStart, timeEnd, 0.25f, std::make_shared<Dielectric>(1.5f)));

    //glass sphere
    world.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, -2.0f), timeStart, timeEnd, 0.5f, std::make_shared<Dielectric>(1.5f)));
    world.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, -2.0f), Vec3(0.0f, 0.0f, -2.0f), timeStart, timeEnd, -0.45f, std::make_shared<Dielectric>(1.5f)));

    world.add(std::make_shared<Sphere>(Vec3(0.0f, 1.2f, -2.0f), Vec3(0.0f, 1.2f, -2.0f), timeStart, timeEnd, 
              0.5f, std::make_shared<Lambertian>(earthTexture)));*/

    BVHNode root(world, 0.001f, infinity);


    Point3 lookFrom(-1.0f, 0.5f, 1.0f);
    Point3 lookAt(0.0f, 0.0f, -2.0f);
    float disToFocus = (lookFrom - lookAt).length();
    float aperture = 0.03f;
    float fov = 50.0f;
    Vec3 vUp(0.0f, 1.0f, 0.0f);
    float nxf = static_cast<float>(nx);
    float nyf = static_cast<float>(ny);
    Camera cam(lookFrom, lookAt, vUp, fov, nxf / nyf, aperture, disToFocus, timeStart, timeEnd);

    for (int j = ny - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < nx; ++i)
        {
            Color col(0.0f, 0.0f, 0.0f);

            for (int k = 0; k < ns; ++k) //sample ns times for antialiasing
            {
                float u = (static_cast<float>(i) + randf()) / nxf;
                float v = (static_cast<float>(j) + randf()) / nyf;
                Ray r = cam.getRay(u, v);
                col += color(r, root, max_depth); //check all objects
            }

            write_color(imageFile, col, ns);
        }
    }

    std::cerr << "\nDone.\n"
              << std::flush;

    imageFile.close();
    return 0;
}