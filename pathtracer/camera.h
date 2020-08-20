#ifndef CAMERA_H
#define CAMERA_H

#include "utilities.h"

class camera {
public:
    camera(point3 lookfrom,
           point3 lookat,
           vec3 vup, // up vector for the camera
           double vfov, // fvov : vertical field-of-view in degrees
           double aspect_ratio,
           double aperture,
           double focus_dist,
           double t0, // t0 to t1 is the time the camera shutter stays open for motion blur
           double t1) {
        auto theta = degrees_to_radians(vfov);
        auto h = focus_dist * tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;
        lens_radius = aperture / 2;
        time0 = t0;
        time1 = t1;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset,
                   lower_left_corner + s*horizontal + t*vertical - origin - offset,
                   random_double(time0, time1));
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double time0, time1;
};

#endif