#ifndef SHAPE_H
#define SHAPE_H 1

#include "../core/ray.h"
#include "../core/surfel.h"
#include "../core/transform.h"
#include "../primitives/bounds.h"

namespace rt3{

// The Shape is the base class for low level object classes like triangle, sphere, box, etc., 
// and provides access to the raw geometric properties of the object, such as the bounding box 
// and a ray intersection routine.
class Shape {
public:
    // TODO: Change constructor to update Tranforms
    Shape(bool flip_n);
    virtual Bounds3f world_bounds() = 0;
    virtual bool intersect(const Ray& r, float *t_hit, Surfel *isect) const = 0;
    virtual bool intersect_p( const Ray& r ) const = 0;

    // === Shape Public Data
    const Transform * obj_to_world; //!< Object to World transformation (scene specified).
    const Transform * world_to_obj; //!< World to Object trasformation (deduced base on O2W).
    const bool flip_normals;
    
};

} // namespace rt3

#endif