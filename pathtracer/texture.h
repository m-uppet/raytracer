#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"

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
        return color(1, 1, 1) * noise.noise(scale * p);
    }

public:
    perlin noise;
    double scale;
};

#endif