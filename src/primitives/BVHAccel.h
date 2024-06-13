#ifndef BVHAccel_H
#define BVHAccel_H 1
#include "../core/rt3.h"
#include "../core/ray.h"
#include "primitive.h"
namespace rt3{
class BVHAccel: public AggregatePrimitive{
private:
    std::vector<std::shared_ptr<Primitive>> primitives;
public:
    // PrimList(std::vector<std::shared_ptr<Primitive>> prim):
    BVHAccel(){ /* empty */ };
    bool intersect( const Ray& r, Surfel *sf ) const;
	// Simpler & faster version of intersection that only return true/false.
	// It does not compute the hit point information.
	bool intersect_p( const Ray& r ) const;
	Bounds3f world_bounds();
    std::vector<std::shared_ptr<Primitive>> get_primitives() const {return primitives;};

public:
    // BVHAccel(PrimList list) : BVHAccel(list.objects, 0, list.objects.size()) {
    //     // There's a C++ subtlety here. This constructor (without span indices) creates an
    //     // implicit copy of the hittable list, which we will modify. The lifetime of the copied
    //     // list only extends until this constructor exits. That's OK, because we only need to
    //     // persist the resulting bounding volume hierarchy.
    // }

    BVHAccel(std::vector<std::shared_ptr<Primitive>>& objects);

    bool intersect_p(const Ray& r) const;
    bool intersect( const Ray& r, Surfel *sf );

    Bounds3f world_bounds() const { return bbox; }

private:
    std::shared_ptr<Primitive> left;
    std::shared_ptr<Primitive> right;
    Bounds3f bbox;
};
} // namespace rt3
#endif