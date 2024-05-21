#ifndef SIMPLE_TRIANGLE_H
#define SIMPLE_TRIANGLE_H 1

#include "../core/rt3.h"
#include "../primitives/primitive.h"
#include "../core/paramset.h"

namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

/*
    This is a simple triangle :D (it has 3 points and a normal)
*/
class SimpleTriangle : public Shape{
private:
    Point3f p0, p1, p2;
    Vector3f norm;
public:
    SimpleTriangle(const bool& flip_n, const Point3f& p0, const Point3f& p1, const Point3f& p2, const Vector3f& n);
    SimpleTriangle(const bool& flip_n, const Point3f& po0, const Point3f& po1, const Point3f& po2);
    Bounds3f world_bounds();

    bool intersect_p( const Ray& r ) const;
    bool intersect( const Ray& r, float *t_hit, Surfel *sf ) const;

    virtual ~SimpleTriangle(){};

};

//-------------------------------------------------------------------------------
// Factory pattern. It's not part of this class.
SimpleTriangle* create_simple_triangle(const ParamSet &ps);
} // namespace rt3

#endif