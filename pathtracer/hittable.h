#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.h"
#include "utilities.h"
#include "ray.h"

class material;

/* front_face boolean records which side of the surface the ray hits, so as to have the normal
always facing against the ray. Alternative solution : normal always points to the outside, and we
calculate the dot product of the ray and the normal to determine which side of the surface the ray
hits.*/
struct hit_record {
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    double u;
    double v; // u,v used for texture mapping (see UV mapping)
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
public:
    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const = 0;
    virtual bool bounding_box(aabb& output_box, double t0, double t1) const = 0;
};

#endif