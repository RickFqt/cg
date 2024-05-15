#include "primlist.h"

namespace rt3 {

bool PrimList::intersect_p( const Ray& r ) const{

    for(auto e : primitives){
        if(e->intersect_p(r)){
            return true;
        }
    }
    return false;
}

bool PrimList::intersect( const Ray& r, Surfel *sf ) const{
    
    bool has_intersected = false;
    for(std::shared_ptr<Primitive> e : primitives){
        if(e->intersect(r, sf)){
            has_intersected = true;
        }
    }
    return has_intersected;
}

// Bounds3f PrimList::world_bounds(){

//     if(initialized){
//         return global_bounds;
//     }

//     real_type min_x, min_y, min_z, max_x, max_y, max_z;
//     min_x = min_y = min_z = FLT_MAX;
//     max_x = max_y = max_z = FLT_MIN;

//     for(std::shared_ptr<Primitive> p : primitives){
//         Bounds3f bounds = p->world_bounds();
//         Point3f low = bounds.get_low();
//         if(low.x < min_x){
//             min_x = low.x;
//         }
//         if(low.y < min_y){
//             min_y = low.y;
//         }
//         if(low.z < min_z){
//             min_z = low.z;
//         }

//         Point3f high = bounds.get_high();
//         if(high.x > max_x){
//             max_x = high.x;
//         }
//         if(high.y > max_y){
//             max_y = high.y;
//         }
//         if(high.z > max_z){
//             max_z = high.z;
//         }
//     }

//     global_bounds.set_low({min_x, min_y, min_z});
//     global_bounds.set_high({max_x, max_y, max_z});
//     initialized = true;

//     std::cout << "GLobal Bounds: " << 
//     "{"<< min_x<<","<< min_y<<","<< min_z<<"}, " <<
//     "{"<< max_x<<","<< max_y<<","<< max_z<<"}\n";

//     return Bounds3f({min_x, min_y, min_z}, {max_x, max_y, max_z});
// }

Bounds3f PrimList::world_bounds(){

    float max_r = FLT_MIN;
    float max_distance = FLT_MIN;
    Point3f new_center = {0,0,0};
    for(std::shared_ptr<Primitive> p : primitives){
        if(p->world_bounds().radius > max_r){
            max_r = p->world_bounds().radius;
        }
        new_center += p->world_bounds().center;
        
    }

    new_center = (new_center / (float)primitives.size());

    float curr_distance;
    for(std::shared_ptr<Primitive> p : primitives){
        curr_distance = glm::distance(p->world_bounds().center, new_center);
        if(curr_distance > max_distance){
            max_distance = curr_distance;
        }
        
    }

    global_bounds.center = new_center;
    global_bounds.radius = max_distance + max_r + 2;
    initialized = true;

    return Bounds3f(false, max_distance + max_r + 2, new_center);
}

}