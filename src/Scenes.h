#ifndef SCENES_H
#define SCENES_H

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
    std::shared_ptr<DiffuseLight> light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(15.0f, 15.0f, 15.0f));

    objects.add(std::make_shared<FlipFace>(std::make_shared<YZRect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green)));
    objects.add(std::make_shared<YZRect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red));
    objects.add(std::make_shared<FlipFace>(std::make_shared<XZRect>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light)));
    objects.add(std::make_shared<FlipFace>(std::make_shared<XZRect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white)));
    objects.add(std::make_shared<XZRect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));
    objects.add(std::make_shared<FlipFace>(std::make_shared<XYRect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white)));

    //objects inside box
    std::shared_ptr<Hitable> box1 = std::make_shared<Box>(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 330.0f, 165.0f), white);
    box1 = std::make_shared<RotateY>(box1, 15.0f);
    box1 = std::make_shared<Translate>(box1, Vec3(265.0f, 0.0f, 295.0f));
    objects.add(box1);

    std::shared_ptr<Hitable> box2 = std::make_shared<Box>(Point3(0.0f, 0.0f, 0.0f), Point3(165.0f, 165.0f, 165.0f), white);
    box2 = std::make_shared<RotateY>(box2, -18.0f);
    box2 = std::make_shared<Translate>(box2, Vec3(130.0f, 0.0f, 65.0f));
    objects.add(box2);

    //objects.add(std::make_shared<ConstantMedium>(box1, 0.01f, std::make_shared<SolidColor>(0.0f, 0.0f, 0.0f)));
    //objects.add(std::make_shared<ConstantMedium>(box2, 0.01f, std::make_shared<SolidColor>(1.0f, 1.0f, 1.0f)));

    return objects;
}


#endif //SCENES_H