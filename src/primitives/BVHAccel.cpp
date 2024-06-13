#include "BVHAccel.h"

namespace rt3{

    static bool box_compare(
        const std::shared_ptr<Primitive> a, const std::shared_ptr<Primitive> b, int axis_index
    ) {
        float a_axis_interval = a->world_bounds().get_p_min()[axis_index];
        float b_axis_interval = b->world_bounds().get_p_min()[axis_index];
        return a_axis_interval < b_axis_interval;
    }

    static bool box_x_compare (const std::shared_ptr<Primitive> a, const std::shared_ptr<Primitive> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare (const std::shared_ptr<Primitive> a, const std::shared_ptr<Primitive> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare (const std::shared_ptr<Primitive> a, const std::shared_ptr<Primitive> b) {
        return box_compare(a, b, 2);
    }

    BVHAccel::BVHAccel(std::vector<std::shared_ptr<Primitive>>& objects){

        // Build the bounding box of the span of source objects.
        Bounds3f global_bounding_box({FLT_MAX, FLT_MAX, FLT_MAX}, {FLT_MIN, FLT_MIN, FLT_MIN});
        for (std::shared_ptr<Primitive> p : objects)
            global_bounding_box = Bounds3f(global_bounding_box, p->world_bounds());

        bbox = global_bounding_box;

        int axis = bbox.largest_extent();

        if(axis == 0){
            std::sort(objects.begin(), objects.end(), box_x_compare);
        }
        else if(axis == 1){
            std::sort(objects.begin(), objects.end(), box_x_compare);
        }else{
            std::sort(objects.begin(), objects.end(), box_x_compare);
        }

        //TODO: Chamar recursivamente aqui


    }

    bool BVHAccel::intersect_p(const Ray& r/*const ray& r, interval ray_t, hit_record& rec*/) const{

        real_type t1{0};
        real_type t2{0};
        if (!bbox.intersect_p(r, &t1, &t2))
            return false;

        bool hit_left = left->intersect_p(r);
        bool hit_right = right->intersect_p(r);

        return hit_left || hit_right;
    }

    bool BVHAccel::intersect(const Ray& r, Surfel *sf) const{

        float t1, t2;
        if (!bbox.intersect_p(r, &t1, &t2))
            return false;

        bool hit_left = left->intersect(r,sf);
        bool hit_right = right->intersect(r,sf);

        return hit_left || hit_right;
        return true;
    }
}