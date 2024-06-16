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

    BVHAccel::BVHAccel(const int& l, const int& r, const std::vector<std::shared_ptr<Primitive>>& objects, const int& max_prims)
    : max_prims_per_node{max_prims}{

        // Build the bounding box of the span of source objects.
        Bounds3f global_bounding_box({FLT_MAX, FLT_MAX, FLT_MAX}, {FLT_MIN, FLT_MIN, FLT_MIN});
        std::shared_ptr<Primitive> p;
        for(int i = l; i < r; ++i){
            p = objects[i];
            // Save the primitives
            primitives.push_back(p);
            global_bounding_box = Bounds3f(global_bounding_box, p->world_bounds());
        }
        // for (std::shared_ptr<Primitive> p : objects)
        //     global_bounding_box = Bounds3f(global_bounding_box, p->world_bounds());

        bbox = global_bounding_box;

        if(r - l == 1){ // TODO: Ver qual é o ponto de parada certo lido do parser
            return;
        }

        int axis = bbox.largest_extent();

        if(axis == 0){
            std::sort(primitives.begin(), primitives.end(), box_x_compare);
        }else if(axis == 1){
            std::sort(primitives.begin(), primitives.end(), box_y_compare);
        }else{
            std::sort(primitives.begin(), primitives.end(), box_z_compare);
        }

        int mid = (l + r)/2;
        left = ((std::shared_ptr<BVHAccel>) new BVHAccel(l, mid, objects, max_prims));
        right = ((std::shared_ptr<BVHAccel>) new BVHAccel(mid, r, objects, max_prims));


    }

    bool BVHAccel::intersect_p(const Ray& r/*const ray& r, interval ray_t, hit_record& rec*/) const{

        real_type t1{0};
        real_type t2{0};
        // std::cout << "abacate\n";
        if (!bbox.intersect_p(r, &t1, &t2))
            return false;
        
        if(left == nullptr && right == nullptr){
            for(auto e : primitives){
                if(e->intersect_p(r)){
                    return true;
                }
            }
            return false;
        }

        bool hit_left = false;
        bool hit_right = false;
        if(left != nullptr){
            hit_left = left->intersect_p(r);
        }
        if(right != nullptr && !hit_left){
            hit_right = right->intersect_p(r);
        }

        return hit_left || hit_right;
    }

    bool BVHAccel::intersect(const Ray& r, Surfel *sf) const{

        // std::cout << "banana1\n";
        real_type t1{0};
        real_type t2{0};
        if (!bbox.intersect_p(r, &t1, &t2))
            return false;
        // std::cout << "banana2\n";
        
        if(left == nullptr && right == nullptr){
            // We've reached the bottom of the tree
            bool has_intersected = false;
            for(std::shared_ptr<Primitive> e : primitives){
                if(e->intersect(r, sf)){
                    has_intersected = true;
                }
            }
            return has_intersected;
        }

        // std::cout << "banana3\n";

        bool hit_left = false;
        bool hit_right = false;
        if(left != nullptr){
            // std::cout << "vou chamar o da esquerda\n";
            hit_left = left->intersect(r, sf);
            // std::cout << "chamei o da esquerda\n";
        }
        if(right != nullptr && !hit_left){
            // std::cout << "vou chamar o da direita\n";
            hit_right = right->intersect(r, sf);
            // std::cout << "chamei o da direita\n";
        }

        // std::cout << "banana4\n";
        if(left != nullptr && right != nullptr){
            // std::cout << "bla\n";
        }

        return hit_left || hit_right;
    }

    void BVHAccel::print_tree(){

        std::cout << "Tenho " << primitives.size() << "primitives." << std::endl;
        std::cout << "inutil: " << max_prims_per_node << "\n";
        std::cout << "Esquerda: ";
        
        if(left != nullptr){
            left->print_tree();
        }

        std::cout << "Direita: ";

        if(right != nullptr){
            right->print_tree();
        }
    }
}// namespace rt3