#ifndef BVHAccel_H
#define BVHAccel_H 1
#include "../core/rt3.h"
#include "../core/ray.h"
#include "primitive.h"
#include "primlist.h"
namespace rt3{
class BVHAccel: public AggregatePrimitive{
private:
    std::vector<std::shared_ptr<Primitive>> primitives;
public:
    // PrimList(std::vector<std::shared_ptr<Primitive>> prim):
    BVHAccel(){ /* empty */ };

public:
    BVHAccel(PrimList list) : BVHAccel(0, list.get_primitives().size(), list.get_primitives()) {
        // There's a C++ subtlety here. This constructor (without span indices) creates an
        // implicit copy of the hittable list, which we will modify. The lifetime of the copied
        // list only extends until this constructor exits. That's OK, because we only need to
        // persist the resulting bounding volume hierarchy.
    }

    BVHAccel(int l, int r, const std::vector<std::shared_ptr<Primitive>>& objects);

    bool intersect_p(const Ray& r) const;
    bool intersect( const Ray& r, Surfel *sf ) const;

    Bounds3f world_bounds() { return bbox; }

private:
    std::shared_ptr<Primitive> left = nullptr;
    std::shared_ptr<Primitive> right = nullptr;
    Bounds3f bbox;
};
} // namespace rt3
#endif