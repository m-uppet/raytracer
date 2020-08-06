#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 c, double r) : center(c), radius(r) {}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    // The discriminant of the ray-circle intersection equation gives the number of intersection
    // points : 0, 1 or 2. In this case we only check if there is intersection or not.
    if (discriminant > 0 ) {
        auto root = sqrt(discriminant);

        // Check if the hit is within the allowed interval t_min to t_max
        auto temp = (-half_b - root)/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center)/radius; // The normal is of length 1
            rec.set_face_normal(r, outward_normal);
            return true;
        }

        temp = (-half_b + root)/a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center)/radius; // The normal is of length 1
            rec.set_face_normal(r, outward_normal);
            return true;
        }
    }

    return false;
}

#endif