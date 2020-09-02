#ifndef TEXTURE_H
#define TEXTURE_H

#include "utilities.h"
#include "perlin.h"
#include "my_stb_image.h"

#include <iostream>

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_texture : public texture {
public:
    solid_texture() {}
    solid_texture(color c) : color_value(c) {}

    solid_texture(double red, double green, double blue)
        : solid_texture(color(red, green, blue)) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) : even(t0), odd(t1) {}

    checker_texture(color c0, color c1)
        : even(make_shared<solid_texture>(c0)), odd(make_shared<solid_texture>(c1)) {}

    virtual color value(double u, double v, const point3& p) const override {
        // trig function products are negative or positive periodically
        auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        }
        return even->value(u, v, p);
    }
public:
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(double sc) : scale(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
        // Make sure the return value is between 0 and 1 to avoid NaNs in color.h sqrt() in the
        // gamma function
        return color(1, 1, 1) * 0.5 * (1.0 + sin(scale*p.z() + 10.0 * noise.turb(scale * p)));
    }

public:
    perlin noise;
    double scale;
};

class image_texture : public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture() : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_pixel;

        data = stbi_load(
                   filename, &width, &height, &components_per_pixel, components_per_pixel);

        if (!data) {
            std::cerr << "ERROR : Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }
        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {
        delete data;
    }

    virtual color value(double u, double v, const point3& p) const override {
        // Return solid color cyan if no texture data.
        if (data == nullptr) {
            return color(0, 1, 1);
        }

        // Clamp input texture coordinates to [0, 1] * [1, 0] so that origin is upper left corner.
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);

        auto i = static_cast<int>(u*width);
        auto j = static_cast<int>(v*height);

        // Making sure the integer values are within bounds
        if (i >= width) {
            i = width - 1;
        }
        if (j >= height) {
            j = height - 1;
        }

        const auto color_scale = 1.0 / 255.0;
        auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;

        return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
    }

private:
    unsigned char* data;
    int width, height;
    int bytes_per_scanline;

};

#endif