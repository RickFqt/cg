#ifndef BVHAccel_H
#define BVHAccel_H 1
#include "../core/rt3.h"
#include "../core/ray.h"
#include "primitive.h"
#include "bounds.h"
namespace rt3{
class BVHAccel: public AggregatePrimitive{
private:
    std::vector<std::shared_ptr<Primitive>> primitives;
    int max_prims_per_node = 1; // TODO: Change this
    std::shared_ptr<Primitive> left = nullptr;
    std::shared_ptr<Primitive> right = nullptr;
    Bounds3f bbox;
public:
    BVHAccel(const int& l, const int& r, const std::vector<std::shared_ptr<Primitive>>& objects, const int& max_prims);

    BVHAccel(const int& l, const int& r, const std::vector<std::shared_ptr<Primitive>>& objects);

    bool intersect_p(const Ray& r) const;
    bool intersect( const Ray& r, Surfel *sf ) const;

    Bounds3f world_bounds() { return bbox; }

};
} // namespace rt3
#endif