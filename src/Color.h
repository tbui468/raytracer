#ifndef COLOR_H
#define COLOR_H


void write_color(std::ofstream &os, const Color& color, int samples)
{
    Color c = color / static_cast<float>(samples);
    os << static_cast<int>(256.0f * clamp(sqrt(c.x()), 0.0f, 0.999f)) << ' '
       << static_cast<int>(256.0f * clamp(sqrt(c.y()), 0.0f, 0.999f)) << ' '
       << static_cast<int>(256.0f * clamp(sqrt(c.z()), 0.0f, 0.999f)) << '\n';
}

#endif //COLOR_H