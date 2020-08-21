#ifndef BVH_H
#define BVH_H

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

#endif