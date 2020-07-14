#ifndef COLOR_H
#define COLOR_H


void write_color(std::ofstream &os, const Color& color, int samples)
{
    float r = color.x();
    float g = color.y();
    float b = color.z();


    float scale = 1.0f / float(samples);

    //get rid of acne form NaN
    //NaN are not equal to themselves (not reflexive for you math peeps)
    /*
    if(r != r) std::cout << "r is Nan" << std::endl;
    if(g != g) std::cout << "g is Nan" << std::endl;
    if(b != b) std::cout << "b is Nan" << std::endl;

    if(r < 0.0f) std::cout << "r negative number" << std::endl;
    if(g < 0.0f) std::cout << "g negative number" << std::endl;
    if(b < 0.0f) std::cout << "b negative number" << std::endl;*/

    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

/*
    if(r != r) std::cout << "sqrt r is Nan" << std::endl;
    if(g != g) std::cout << "sqrt g is Nan" << std::endl;
    if(b != b) std::cout << "sqrt b is Nan" << std::endl;*/


    os << static_cast<int>(256.0f * clamp(r, 0.0f, 0.999f)) << ' '
       << static_cast<int>(256.0f * clamp(g, 0.0f, 0.999f)) << ' '
       << static_cast<int>(256.0f * clamp(b, 0.0f, 0.999f)) << '\n';
}

#endif //COLOR_H