#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "utilities.h"
#include "sphere.h"

class moving_sphere : public hittable {
public:
    moving_sphere() {}
    moving_sphere(
        point3 c0, point3 c1, double t0, double t1, double r, shared_ptr<material> m) :
        center0(c0), center1(c1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box, double t0, double t1) const override;


    point3 center(double time) const;

public:
    point3 center0, center1;
    double time0, time1;
    double radius;
    shared_ptr<material> mat_ptr;
};

point3 moving_sphere::center(double time) const {
    return center0 + (time - time0)*(center1 - center0)/(time1 - time0);
}

bool moving_sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    // The discriminant of the ray-circle intersection equation gives the number of intersection
    // points : 0, 1 or 2. In this case we only check if there is intersection or not.
    if (discriminant > 0 ) {
        auto root = sqrt(discriminant);

        // Check if the hit is within the allowed interval tmin to tmax
        auto temp = (-half_b - root)/a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center(r.time()))/radius; // The normal is of length 1
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-half_b + root)/a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center(r.time()))/radius; // The normal is of length 1
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

bool moving_sphere::bounding_box(aabb& output_box, double t0, double t1) const {
    aabb output_box0 = aabb(
        center(t0) - vec3(radius, radius, radius),
        center(t0) + vec3(radius, radius, radius));
    aabb output_box1 = aabb(
        center(t1) - vec3(radius, radius, radius),
        center(t1) + vec3(radius, radius, radius));

    output_box = surrounding_box(output_box0, output_box1);        
    return true;
}

#endif