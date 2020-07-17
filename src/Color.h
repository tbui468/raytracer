#ifndef COLOR_H
#define COLOR_H


void write_color(std::ofstream &os, const Color& color, int samples)
{
    float r = color.x();
    float g = color.y();
    float b = color.z();


    float scale = 1.0f / float(samples);


    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);



    os << static_cast<int>(256.0f * clamp(r, 0.0f, 0.999f)) << ' '
       << static_cast<int>(256.0f * clamp(g, 0.0f, 0.999f)) << ' '
       << static_cast<int>(256.0f * clamp(b, 0.0f, 0.999f)) << '\n';
}

#endif //COLOR_H