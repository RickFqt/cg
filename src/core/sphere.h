#ifndef SPHERE_H
#define SPHERE_H 1

#include "./rt3.h"
#include "ray.h"
#include "primitive.h"
#include "surfel.h"
#include "paramset.h"

namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

/*
    This is a sphere :D (it has a center and radius)
*/
class Sphere : public Primitive{
private:
    float radius;
    Point3f center;
public:
    Sphere(const float& r, const Point3f& c):radius{r}, center{c}{ /* empty */ }

    bool intersect_p( const Ray& r ) const;
    bool intersect( const Ray& r, Surfel *sf ) const;

    virtual ~Sphere(){};

};

//-------------------------------------------------------------------------------
// Factory pattern. It's not part of this class.
Sphere* create_sphere(const ParamSet &ps);
} // namespace rt3

#endif