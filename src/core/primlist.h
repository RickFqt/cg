#ifndef PRIMLIST_H
#define PRIMLIST_H 1

#include "primitive.h"
#include "ray.h"
#include "surfel.h"

namespace rt3 {

class PrimList : public AggregatePrimitive{
private:
    std::vector<std::shared_ptr<Primitive>> primitives;
public:
    PrimList(std::vector<std::shared_ptr<Primitive>> prim):
    primitives{prim}
    { /* empty */ } 
    bool intersect( const Ray& r, Surfel *sf ) const;
	// Simpler & faster version of intersection that only return true/false.
	// It does not compute the hit point information.
	bool intersect_p( const Ray& r ) const;
	// TODO: Bounds3f world_bounds();

};

//-------------------------------------------------------------------------------
} // namespace rt3


#endif