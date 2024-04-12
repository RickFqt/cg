#include "primlist.h"

namespace rt3 {

bool PrimList::intersect_p( const Ray& r ) const{

    bool has_intersected = false;
    for(auto e : primitives){
        if(e->intersect_p(r)){
            has_intersected = true;
        }
    }
    return has_intersected;
}

bool PrimList::intersect( const Ray& r, Surfel *sf ) const{
    
    bool has_intersected = false;
    float t_max = r.get_t_max();
    for(std::shared_ptr<Primitive> e : primitives){
        if(e->intersect(r, sf)){
            has_intersected = true;

            if(t_max < r.get_t_max()){
                t_max = r.get_t_max();
                sf->primitive = e;
            }
        }
    }
    return has_intersected;
}

// TODO: Bounds3f PrimList::world_bounds(){}

}