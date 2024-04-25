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

// TODO: Bounds3f PrimList::world_bounds(){}

}