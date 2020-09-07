#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 c, double r, shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {}

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;

    virtual bool bounding_box(double t0, double t1, aabb& output_box) const override;

public:
    point3 center;
    double radius;
    shared_ptr<material> mat_ptr;
};

// Maps x,y,z coordinates on unit sphere to u,v coordinates
void get_sphere_uv(const point3& p, double& u, double& v) {
    // u and v should be between 0 and 1
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1 - (phi + pi)/(2*pi);
    v = (theta + pi/2) / pi;
}

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
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
            vec3 outward_normal = (rec.p - center)/radius; // The normal is of length 1
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-half_b + root)/a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center)/radius; // The normal is of length 1
            rec.set_face_normal(r, outward_normal);
            get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

bool sphere::bounding_box(double t0, double t1, aabb& output_box) const {
    output_box = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));
    return true;
}

#endif