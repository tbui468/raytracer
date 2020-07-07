#include <iostream>
#include <fstream>


#include "RayTracer.h" //common headers (including stb_image)
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "Color.h"
#include "BVH.h"
#include "AARect.h"
#include "Box.h"
#include "ConstantMedium.h"

//first book cover w/ matte, metal, glass, hollow glass spheres
//is this really necessary?????
HitableList marbles_scene() {
    HitableList objects;
    return objects;
}

HitableList depth_blur_scene() {
    HitableList objects;
    //world
    std::shared_ptr<NoiseTexture> marble = std::make_shared<NoiseTexture>(1.0f);
    std::shared_ptr<SolidColor> red = std::make_shared<SolidColor>(0.9f, 0.5f, 0.5f);
    std::shared_ptr<SolidColor> green = std::make_shared<SolidColor>(0.5f, 0.9f, 0.5f);
    std::shared_ptr<SolidColor> blue = std::make_shared<SolidColor>(0.5f, 0.5f, 0.9f);
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, -200.5f, -1.0f), Vec3(0.0f, -200.5f, -1.0f), 0.0f, 1.0f, 200.0f, std::make_shared<Lambertian>(marble)));
    //moving spheres (two vertical and one horizontal)
    objects.add(std::make_shared<Sphere>(Vec3(-3.0f, 1.5f, -3.0f), Vec3(-3.0f, 1.5f, -3.0f), 0.0f, 1.0f, 
              2.0f, std::make_shared<Lambertian>(red)));
    objects.add(std::make_shared<Sphere>(Vec3(1.0f, 0.5f, -4.0f), Vec3(1.0f, 0.5f, -4.0f), 0.0f, 1.0f, 
              1.0f, std::make_shared<Lambertian>(blue)));
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.5f, 0.0f), Vec3(0.0f, 0.5f, 0.0f), 0.0f, 1.0f, 
              1.0f, std::make_shared<Lambertian>(green)));
    objects.add(std::make_shared<Sphere>(Vec3(1.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, -1.0f), 0.0f, 1.0f, 
              0.5f, std::make_shared<Dielectric>(1.5f)));
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, 2.0f), 0.0f, 1.0f, 0.5f, std::make_shared<Dielectric>(1.5f)));
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, 2.0f), 0.0f, 1.0f, -.45f, std::make_shared<Dielectric>(1.5f)));
    return objects;
}

//checker ground, solid sphere, image-texture sphere, marble sphere, metal sphere (large)
HitableList texture_scene() {
    HitableList objects;
    std::shared_ptr<CheckerTexture> checker = std::make_shared<CheckerTexture>(
              std::make_shared<SolidColor>(1.0f, 1.0f, 1.0f), 
              std::make_shared<SolidColor>(0.2f, 0.2f, 0.2f));

    std::shared_ptr<NoiseTexture> marble = std::make_shared<NoiseTexture>(1.0f);

    std::shared_ptr<SolidColor> green = std::make_shared<SolidColor>(0.7f, 0.9f, 0.5f);

    std::shared_ptr<ImageTexture> earthTexture = std::make_shared<ImageTexture>("../../../src/earthmap.jpg");


    objects.add(std::make_shared<Sphere>(Vec3(0.0f, -200.5f, -1.0f), Vec3(0.0f, -200.5f, -1.0f), 0.0f, 1.0f, 200.0f, std::make_shared<Lambertian>(checker)));
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f, 
              0.5f, std::make_shared<Lambertian>(green)));
    objects.add(std::make_shared<Sphere>(Vec3(-1.0f, 1.5f, -2.0f), Vec3(-1.0f, 1.5f, -2.0f), 0.0f, 1.0f, 
              0.8f, std::make_shared<Lambertian>(earthTexture)));
    objects.add(std::make_shared<Sphere>(Vec3(-1.0f, 0.5f, 0.0f), Vec3(-1.0f, 0.5f, 0.0f), 0.0f, 1.0f, 
              1.0f, std::make_shared<Lambertian>(marble)));

    //objects.add(std::make_shared<Sphere>(Vec3(1.5f, 0.5f, -1.25f), Vec3(1.5f, 0.5f, -1.25f), 0.0f, 1.0f, 1.0f, std::make_shared<Metal>(Vec3(1.0f, 0.5f, 0.7f), 0.01f)));
    objects.add(std::make_shared<Sphere>(Vec3(1.5f, 0.5f, -1.25f), Vec3(1.5f, 0.5f, -1.25f), 0.0f, 1.0f, 1.0f, std::make_shared<Metal>(marble, 0.11f)));


    return objects;
}

//three moving spheres (red, blue and green)
HitableList motion_blur_scene() {
    HitableList objects;
    //world
    std::shared_ptr<NoiseTexture> marble = std::make_shared<NoiseTexture>(1.0f);
    std::shared_ptr<SolidColor> red = std::make_shared<SolidColor>(0.9f, 0.5f, 0.5f);
    std::shared_ptr<SolidColor> green = std::make_shared<SolidColor>(0.5f, 0.9f, 0.5f);
    std::shared_ptr<SolidColor> blue = std::make_shared<SolidColor>(0.5f, 0.5f, 0.9f);
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, -200.5f, -1.0f), Vec3(0.0f, -200.5f, -1.0f), 0.0f, 1.0f, 200.0f, std::make_shared<Lambertian>(marble)));
    //moving spheres (two vertical and one horizontal)
    objects.add(std::make_shared<Sphere>(Vec3(-1.5f, 0.5f, 0.0f), Vec3(-1.5f, 1.0f, 0.0f), 0.0f, 1.0f, 
              0.7f, std::make_shared<Lambertian>(red)));
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.5f, 0.0f), Vec3(0.0f, 2.0f, 0.0f), 0.0f, 1.0f, 
              0.7f, std::make_shared<Lambertian>(green)));
    objects.add(std::make_shared<Sphere>(Vec3(1.5f, 0.7f, 0.0f), Vec3(1.5f, 0.5f, 0.0f), 0.0f, 1.0f, 
              0.7f, std::make_shared<Lambertian>(blue)));
    //objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, 2.0f), Vec3(0.0f, 0.0f, 2.0f), 0.0f, 1.0f, 0.5f, std::make_shared<Metal>(Vec3(1.0f, 0.7f, 1.0f), 0.1f)));
    return objects;
}

HitableList light_scene() {
    HitableList objects;

    std::shared_ptr<NoiseTexture> marble = std::make_shared<NoiseTexture>(1.0f);
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(4.0f, 4.0f, 4.0f));

    //light
    objects.add(std::make_shared<XYRect>(-0.5f, 0.5f, 0.5f, 1.5f, -1.5f, light));
    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 3.0f, 0.0f), Vec3(0.0f, 3.0f, 0.0f), 0.0f, 1.0f, 
              1.0f, light));

    objects.add(std::make_shared<Sphere>(Vec3(0.0f, 0.5f, 0.0f), Vec3(0.0f, 0.5f, 0.0f), 0.0f, 1.0f, 
              1.0f, std::make_shared<Lambertian>(marble)));


    objects.add(std::make_shared<Sphere>(Vec3(0.0f, -200.5f, -1.0f), Vec3(0.0f, -200.5f, -1.0f), 0.0f, 1.0f, 200.0f, std::make_shared<Lambertian>(marble)));
    return objects;
}

//cornell box with constant density volumne cubes
HitableList cornell_box() {
    HitableList objects;

    std::shared_ptr<Lambertian> red = std::make_shared<Lambertian>(std::make_shared<SolidColor>(.65f, .05f, .05f));
    std::shared_ptr<Lambertian> white = std::make_shared<Lambertian>(std::make_shared<SolidColor>(.73f, .73f, .73f));
    std::shared_ptr<Lambertian> green = std::make_shared<Lambertian>(std::make_shared<SolidColor>(.12f, .45f, .15f));
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(7.0f, 7.0f, 7.0f));

    objects.add(std::make_shared<FlipFace>(std::make_shared<YZRect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green)));
    objects.add(std::make_shared<YZRect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red));
    objects.add(std::make_shared<XZRect>(113.0f, 443.0f, 127.0f, 432.0f, 554.0f, light));
    objects.add(std::make_shared<FlipFace>(std::make_shared<XZRect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white)));
    objects.add(std::make_shared<XZRect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<XYRect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white)));

    //objects inside box
    std::shared_ptr<Hitable> box1 = std::make_shared<Box>(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 330.0f, 165.0f), white);
    box1 = std::make_shared<RotateY>(box1, 15.0f);
    box1 = std::make_shared<Translate>(box1, Vec3(265.0f, 0.0f, 295.0f));

    std::shared_ptr<Hitable> box2 = std::make_shared<Box>(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 165.0f, 165.0f), white);
    box2 = std::make_shared<RotateY>(box2, -18.0f);
    box2 = std::make_shared<Translate>(box2, Vec3(130.0f, 0.0f, 65.0f));

    objects.add(std::make_shared<ConstantMedium>(box1, 0.01f, std::make_shared<SolidColor>(0.0f, 0.0f, 0.0f)));
    objects.add(std::make_shared<ConstantMedium>(box2, 0.01f, std::make_shared<SolidColor>(1.0f, 1.0f, 1.0f)));

    return objects;
}




//returns a Vec3 color
//depth is the maximum number of reflections
Vec3 ray_color(const Ray &r, const Color& background, const Hitable& world, int depth)
{
    HitRecord rec;
    if(world.hit(r, 0.001f, infinity, rec)) { //check for any hits in world.  If so, fills in HitRecord rec
        Ray scattered;
        Vec3 attenuation;
        if(depth >= 0){ 
            Color emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);
            if(rec.material_ptr->scatter(r, rec, attenuation, scattered)) { //if scatter
                return emitted + attenuation * ray_color(scattered, background, world, depth - 1); //recursively checks world again using new scattered light ray
            }else{ //if no scattering
                return emitted;
            }
        }else{
            return Vec3(0.0f, 0.0f, 0.0f); //returns black if scatter depth is < 0 or if light is scattered to inside of metal
        }
    }
    else //returns background color if no Hitables are hit
    {
        return background;
    }
}

int main()
{
    std::ofstream imageFile;
    imageFile.open("marble_scene.ppm");

    constexpr int nx = 200;
    constexpr int ny = 200;
    constexpr int ns = 56; //number of samples per pixel
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

    std::shared_ptr<NoiseTexture> marble = std::make_shared<NoiseTexture>(1.0f);

    std::shared_ptr<ImageTexture> earthTexture = std::make_shared<ImageTexture>("../../../src/earthmap.jpg");

    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(4.0f, 4.0f, 4.0f));


    /*
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

    //BVHNode root(world, 0.001f, infinity);
    HitableList cornellBox = cornell_box();
    HitableList lightScene = light_scene();
    HitableList textureScene = texture_scene();
    HitableList motionBlurScene = motion_blur_scene();
    HitableList depthBlurScene = depth_blur_scene();
    HitableList marbleScene = marbles_scene();
    BVHNode root(textureScene, 0.001f, infinity);

    const Color background(0.8f, 0.9f, 1.0f);
    /*
    Point3 lookFrom(6.0f, 3.0f, 2.0f);
    Point3 lookAt(0.0f, 0.0f, 0.0f);
    float disToFocus = (lookFrom - lookAt).length();
    float aperture = 0.03f;
    float fov = 50.0f;
    Vec3 vUp(0.0f, 1.0f, 0.0f);*/
    //Point3 lookFrom(278.0f, 278.0f, -800.0f);
    //Point3 lookAt(278.0f, 278.0f, 0.0f);

    //light
    Point3 lookFrom(2.0f, 1.0f, 5.0f);
    Point3 lookAt(0.0f, 0.5f, 0.0f);
    float disToFocus = 10.0f;
    float aperture = 0.05f;
    float fov = 40.0f;
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
                col += ray_color(r, background, root, max_depth); //check all objects
            }

            write_color(imageFile, col, ns);
        }
    }

    std::cerr << "\nDone.\n"
              << std::flush;

    imageFile.close();
    return 0;
}