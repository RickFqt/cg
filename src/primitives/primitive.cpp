#include "primitive.h"

namespace rt3 {

bool GeometricPrimitive::intersect_p( const Ray& r ) const{

    return shape->intersect_p(r);
}

bool GeometricPrimitive::intersect( const Ray& r, Surfel *sf ) const{

    real_type t_hit{ 0 };
    // Test if I got a hit from this shape
    if (not shape->intersect(r, &t_hit, sf)) {
        return false;
    }

    r.set_t_max(t_hit);
    sf->primitive = this;
    return true;
}

Bounds3f GeometricPrimitive::world_bounds(){

    return shape->world_bounds();
}

}