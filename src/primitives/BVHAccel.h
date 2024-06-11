#ifndef BVAHccel_H
#define BVAHccel_H 1
#include "../core/rt3.h"
#include "../core/ray.h"
#include "primitive.h"
namespace rt3{
class BVAHccel: public AggregatePrimitive{
private:
    std::vector<std::shared_ptr<Primitive>> primitives;
public:
    // PrimList(std::vector<std::shared_ptr<Primitive>> prim):
    BVAHccel(){ /* empty */ };
    bool intersect( const Ray& r, Surfel *sf ) const;
	// Simpler & faster version of intersection that only return true/false.
	// It does not compute the hit point information.
	bool intersect_p( const Ray& r ) const;
	Bounds3f world_bounds();
    std::vector<std::shared_ptr<Primitive>> get_primitives() const {return primitives;};

};
} // namespace rt3
#endif