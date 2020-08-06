#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.h"

struct hit_record;

class material {
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

// Lambertian materials could scatter always and attenuate by reflectance R, or scatter with no
// attenuation and absorb 1-R of rays, or mixture of both. Here it always scatters.
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {};
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 scatter_direction = rec.p + rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a) : albedo(a) {};
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflect_direction = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflect_direction);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
};

#endif