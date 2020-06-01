#include <iostream>
#include <fstream>

int main()
{
    std::ofstream imageFile;
    imageFile.open("image.ppm");

    int nx = 200;
    int ny = 100;

    //P3: ASCII ppm file, width , height, 255: max value
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";

    for(int j = ny - 1; j >= 0; --j) {
        for(int i = 0; i < nx; ++i) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2f;

            int ir = int(255.99f * r);
            int ig = int(255.99f * g);
            int ib = int(255.99f * b);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    imageFile.close();
    return 0;
}