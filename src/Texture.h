#ifndef TEXTURE_H
#define TEXTURE_H

#include "RayTracer.h"
#include <iostream>

class Texture {
public:
    virtual Color value(float u, float v, const Point3& p) const = 0;
};


class SolidColor : public Texture {
public:
    SolidColor() {};
    SolidColor(Color c) : m_colorValue(c) {};
    SolidColor(float r, float g, float b) : SolidColor(Color(r, g, b)) {};
    virtual Color value(float u, float v, const Point3& p) const override {
        return m_colorValue;
    }
private:
    Color m_colorValue;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture() {};
    CheckerTexture(std::shared_ptr<Texture> odd, std::shared_ptr<Texture> even) : m_odd(odd), m_even(even) {};
    virtual Color value(float u, float v, const Point3& p) const override {
        float sines = sin(8.0*p.x()) * sin(8.0f*p.y()) * sin(8.0f*p.z());
        if(sines < 0.0f){
            return m_odd->value(u, v, p);
        }else {
            return m_even->value(u, v, p);
        }
    }
private:
    std::shared_ptr<Texture> m_odd;
    std::shared_ptr<Texture> m_even;
};

class ImageTexture : public Texture
{
public:
    const static int bytes_per_pixel = 3;

    ImageTexture(): m_data(nullptr), m_width(0), m_height(0), m_bytes_per_scanline(0) {};
    ImageTexture(const char* fileName) {
        int components_per_pixel = bytes_per_pixel;
        m_data = stbi_load(fileName, &m_width, &m_height, &components_per_pixel, 0);

        if(!m_data) {
            std::cerr << "Error: Could not load texture image file " << fileName << '\n';
            m_width = 0;
            m_height = 0;
        }

        m_bytes_per_scanline = components_per_pixel * m_width;
    }
    //copy constructor
    ImageTexture(const ImageTexture& other) {
        //copy data
        m_width = other.m_width;
        m_height = other.m_height;
        m_bytes_per_scanline = other.m_bytes_per_scanline;
    }
    //destructor
    ~ImageTexture() {

    }
    //copy assignment
    //move assignment
    //move constructor
    virtual Color value(float u, float v, const Point3& p) const override {
        //return color value based on image saved in m_data
    }
private:
    unsigned char *m_data;
    int m_width;
    int m_height;
    int m_bytes_per_scanline;
};

#endif TEXTURE_H