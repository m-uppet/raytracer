#ifndef AARECT_H
#define AARECT_H

class xy_rect : public hittable {
public:
    xy_rect() {}

    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k,
            shared_ptr<material> mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}

    virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const override;
    virtual bool bounding_box(aabb& output_box, double t0, double t1) const override {
        // The bounding box must have non-zero width in each dimension. Because it is axis aligned
        // like this rect, we must pad z dimension a bit.
        output_box = aabb(point3(x0, y0, k-0.0001), point3(x1, y1, k+0.0001));
        return true;
    }

public:
    shared_ptr<material> mp;
    double x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    t = (k - r.origin().z())/r.direction().z();
    if (t < tmin || t > tmax) {
        return false;
    }
    
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }
    
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}

#endif