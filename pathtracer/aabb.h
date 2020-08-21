#ifndef AABB_H
#define AABB_H

#include "utilities.h"

class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) {
        _min = a;
        _max = b;
    }

    point3 min() const {
        return _min;
    }
    point3 max() const {
        return _max;
    }

    bool hit(const ray& r, double tmin, double tmax) const;
    // Simplest most logical hit method
    // bool hit(const ray& r, double tmin, double tmax) const {
    //     for (int i = 0; i < 3; i++) {
    //         auto t0 = fmin((_min[i] - r.origin()[i]) / r.direction()[i],
    //                        _max[i] -  r.origin()[i] / r.direction()[i]);
    //         auto t1 = fmax((_min[i] - r.origin()[i]) / r.direction()[i],
    //                        _max[i] -  r.origin()[i] / r.direction()[i]);

    //         tmin = fmax(t0, tmin);
    //         tmax = fmin(t1, tmax);
    //         if (tmax <= tmin) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
public:
    point3 _min;
    point3 _max;
};

// Optimized hit method by Andrew Kenslar at Pixar
inline bool aabb::hit(const ray& r, double tmin, double tmax) const {
    for (int i = 0; i < 3; i++) {
        auto invD = 1.0f / r.direction()[i];
        auto t0 = (min()[i] - r.origin()[i]) * invD;
        auto t1 = (max()[i] - r.origin()[i]) * invD;

        if (invD < 0.0f ) {
            std::swap(t0, t1);
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin) {
            return false;
        }
    }
    return true;
}

aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));
    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}

#endif