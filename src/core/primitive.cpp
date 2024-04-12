#include "primitive.h"

namespace rt3 {

bool GeometricPrimitive::intersect_p( const Ray& r ) const{

    return shape->intersect_p(r);
}

bool GeometricPrimitive::intersect( const Ray& r, Surfel *sf ) const{

    bool has_intersected = false;
    float t_hit = r.get_t_max();
    has_intersected = shape->intersect(r, &t_hit, sf);
    r.set_t_max(t_hit);
    //TODO: Update Surfel Here?

    return has_intersected;
}

// TODO: Bounds3f PrimList::world_bounds(){}

}