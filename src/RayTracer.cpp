#include <iostream>
#include <fstream>


#include "RayTracer.h" //common headers (including stb_image)
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"
#include "Color.h"
#include "BVH.h" //bounding volumne hierarchy
#include "AARect.h" //axis-aligned rectangles
#include "Box.h"
#include "ConstantMedium.h" //eg, gas or fog
#include "Scenes.h" //list of scenes
#include "PDF.h"




//returns a Color
//depth is the maximum number of reflections
Color ray_color(const Ray &r, const Color& background, const Hitable& world, int depth)
{
    HitRecord rec;

    //if exceeded max depth
    if(depth <= 0)
        return Color(0.0f, 0.0f, 0.0f);

    //if ray hits no object, return the background color
    if(!world.hit(r, 0.001f, infinity, rec))
        return background;

    ScatterRecord srec;
    //Color emitted = rec.material_ptr->emitted(r, rec, rec.u, rec.v, rec.p); //can simplify to just rec
    Color emitted = rec.material_ptr->emitted(r, rec); //can simplify to just rec
    if(!rec.material_ptr->scatter(r, rec, srec))
        return emitted;
    
    if(srec.is_specular) {
        return srec.attenuation * ray_color(srec.specular_ray, background,
                                            world, depth - 1);
    }

    std::shared_ptr<Hitable> light_shape = 
        std::make_shared<XZRect>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f,
        std::shared_ptr<Material>());
    std::shared_ptr<HittablePDF> p0 = std::make_shared<HittablePDF>(light_shape, rec.p);

    MixturePDF p(p0, srec.pdf_ptr);


    Ray scattered(rec.p, p.generate(), r.time());
    float pdf_val = p.value(scattered.direction());  
    if(pdf_val < 0.0001f) //avoid dividing by zero (and very small numbers)
        return emitted;
    return emitted + srec.attenuation * rec.material_ptr->scattering_pdf(r, rec, scattered) * 
        ray_color(scattered, background, world, depth - 1)/pdf_val;

}

int main()
{
    std::ofstream imageFile;
    imageFile.open("cornell_box.ppm");

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



    HitableList cornellBox = cornell_box();
    HitableList lightScene = light_scene();
    HitableList textureScene = texture_scene();
    HitableList motionBlurScene = motion_blur_scene();
    HitableList depthBlurScene = depth_blur_scene();
    HitableList marbleScene = marbles_scene();
    BVHNode root(cornellBox, 0.001f, infinity);

    const Color background(0.0f, 0.0f, 0.0f);

    /*
    Point3 lookFrom(2.0f, 1.0f, 5.0f);
    Point3 lookAt(0.0f, 0.5f, 0.0f);
    float disToFocus = 10.0f;
    float aperture = 0.05f;
    float fov = 40.0f;
    Vec3 vUp(0.0f, 1.0f, 0.0f);

    float nxf = static_cast<float>(nx);
    float nyf = static_cast<float>(ny);
    Camera cam(lookFrom, lookAt, vUp, fov, nxf / nyf, aperture, disToFocus, timeStart, timeEnd);*/

    Point3 lookfrom(278.0f, 278.0f, -800.0f);
    Point3 lookat(278.0f, 278.0f, 0.0f);
    Vec3 vup(0.0f, 1.0f, 0.0f);
    float dis_to_focus = 10.0f;
    float aperture = 0.0f;
    float vfov = 40.0f;
    float t0 = 0.0f;
    float t1 = 1.0f;
    float nxf = static_cast<float>(nx);
    float nyf = static_cast<float>(ny);

    Camera cam(lookfrom, lookat, vup, vfov, nxf/nyf, aperture, dis_to_focus, t0, t1);



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

            write_color(imageFile, col, ns); //function averages col based on number of samples
        }
    }

    std::cerr << "\nDone.\n"
              << std::flush;

    imageFile.close();
    return 0;
}