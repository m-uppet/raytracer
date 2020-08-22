#ifndef BVH_H
#define BVH_H

#include <algorithm.h>

class bvh_node : public hittable {
public:
    bvh_node();

    bvh_node(hittable_list& list, double time0, double time1)
        : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}

    bvh_node(
        std::vector<shared_ptr<hittable>>& objects,
        size_t start, size_t end, double time0, double time1);

    virtual bool hit(
        const ray& r, double tmin, double tmax, hit_record& rec) const override;

    virtual bool bounding_box(aabb& output_box, double t0, double t1) const override;

public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb box;

};

bool bvh_node::bounding_box(aabb& output_box, double t0, double t1) {
    output_box = box;
    return true;
}

bvh_node::bvh_node(
    std::vector<shared_ptr<hittable>>& objects,
    size_t start, size_t end, double time0, double time1) {
    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                      : (axis == 1) ? box_y_compare
                      : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
        left = right = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    if (  !left->bounding_box (box_left, time0, time1)
            || !right->bounding_box(box_right, time0, time1)
       )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = surrounding_box(box_left, box_right);
}

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(box_a,0,0) || !b->bounding_box(box_b,0,0))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}


bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

#endif