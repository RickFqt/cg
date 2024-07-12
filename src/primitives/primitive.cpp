#include "primitive.h"

namespace rt3 {

bool GeometricPrimitive::intersect_p( const Ray& r ) const{

    // std::cout << "intersectou com alwwwwwwwwwwwwwwwwwwwwwwwwwuma coisa!!!!!!!" << std::endl;

    // if(shape->intersect_p(r)){
    //     std::cout << "intersectou com alguma coisa!!!!!!!" << std::endl;
    //     return true;
    // }

    return shape->intersect_p(r);
}

bool GeometricPrimitive::intersect( const Ray& r, Surfel *sf ) const{

    //  std::cout << "intersectou com alwwwwwwwwwwwwwwwwwwwwwwwwwuma coisa!!!!!!!" << std::endl;
    real_type t_hit{ 0 };
    // Test if I got a hit from this shape
    if (not shape->intersect(r, &t_hit, sf)) {
        return false;
    }

    // std::cout << "intersectou com alguma coisa!!!!!!!" << std::endl;

    r.set_t_max(t_hit);
    sf->primitive = this;
    return true;
}

Bounds3f GeometricPrimitive::world_bounds(){

    // std::cout << "cenoura" << std::endl;

    return shape->world_bounds();
}

}