#ifndef MATERIAL_H
#define MATERIAL_H

#include "utilities.h"
#include "moving_sphere.h"

struct hit_record;

// Shlick approximation to take into account glass's reflectivity varying with angle
double schlick(double cosine, double ref_idx) {
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0)*pow((1-cosine), 5);
}


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
        vec3 scatter_direction = rec.normal + random_unit_vector();
        scattered = ray(rec.p, scatter_direction, r_in.time());
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1 ) {};
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vec3 reflect_direction = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflect_direction + fuzz*random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double ri) : ref_idx(ri) {};
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refract_quotient = rec.front_face ? (1.0 / ref_idx) : ref_idx;
        vec3 unit_direction = unit_vector(r_in.direction());

        // Reflection inside the dielectric surface
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        
        // Schlick approximation
        if (refract_quotient * sin_theta > 1.0
                || (random_double() < schlick(cos_theta, refract_quotient)) ) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = ray(rec.p, reflected, r_in.time());
            return true;
        }

        vec3 refracted = refract(unit_direction, rec.normal, refract_quotient);
        scattered = ray(rec.p, refracted);
        return true;
    }

public:
    double ref_idx;
};

#endif