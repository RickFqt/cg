#ifndef PLANE_H
#define PLANE_H 1

#include "../core/rt3.h"
#include "../primitives/primitive.h"
// #include "../shapes/shape.h"

namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

/*
    This is a sphere :D (it has a center and radius)
*/
class Plane /*: public rt3::Shape*/{
private:
    Vector3f normal;
    Point3f point;
public:

    //TODO: Fix flip_normals
    Plane(/*const bool& flip_n,*/ const Vector3f& v1, const Vector3f& v2):
    /*Shape(flip_n),*/ normal{glm::cross(v1, v2)}, point{v1}
    {};
    // Bounds3f world_bounds();

    bool intersect_p( const Ray& r ) const;
    bool intersect( const Ray& r, float *t_hit, Surfel *sf ) const;

    virtual ~Plane(){};

};

//-------------------------------------------------------------------------------
} // namespace rt3

#endif