#ifndef TRIANGLE_H
#define TRIANGLE_H 1

#include "../core/rt3.h"
#include "../primitives/primitive.h"
#include "../core/paramset.h"

namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

/*
    This is a triangle :D (it has 3 points and a normal)
*/
class Triangle : public Shape{
private:
    Point3f p0, p1, p2;
    Vector3f norm;
public:
    Triangle(const bool& flip_n, const Point3f& p0, const Point3f& p1, const Point3f& p2, const Vector3f& n);
    Triangle(const bool& flip_n, const Point3f& po0, const Point3f& po1, const Point3f& po2);
    Bounds3f world_bounds();

    bool intersect_p( const Ray& r ) const;
    bool intersect( const Ray& r, float *t_hit, Surfel *sf ) const;

    virtual ~Triangle(){};

};

//-------------------------------------------------------------------------------
// Factory pattern. It's not part of this class.
Triangle* create_triangle(const ParamSet &ps);
} // namespace rt3

#endif