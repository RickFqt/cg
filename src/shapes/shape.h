#ifndef SHAPE_H
#define SHAPE_H 1

#include "../core/ray.h"
#include "../core/surfel.h"

namespace rt3{

// The Shape is the base class for low level object classes like triangle, sphere, box, etc., 
// and provides access to the raw geometric properties of the object, such as the bounding box 
// and a ray intersection routine.
class Shape {
private:
    bool flip_normals;

public:
    Shape(bool flip_n):flip_normals{flip_n}{ /*empty*/ };
    // virtual Bounds3f world_bounds TODO
    virtual bool intersect(const Ray& r, float *t_hit, Surfel *isect) const = 0;
    virtual bool intersect_p( const Ray& r ) const = 0;
    
};

} // namespace rt3

#endif