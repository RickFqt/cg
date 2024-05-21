#ifndef BOUNDS_H
#define BOUNDS_H 1

#include "../core/rt3.h"
#include "../core/ray.h"
#include "../shapes/plane.h"
// #include "../shapes/sphere.h"

namespace rt3 {

// class Bounds3f {
// private:
//     Point3f low, high; // Low represents the point with minimum, x, y and z. High the maximum x, y and z

//     Sphere s;

// public:
//     Bounds3f(){}
//     Bounds3f(const Point3f& l, const Point3f& h):
//     low{l}, high{h}
//     { /* empty */ } 
//     bool intersect( const Ray& r, float *t_hit, Surfel *sf ) const;
// 	bool intersect_p( const Ray& r ) const;
//     Point3f get_low() const{return low;}
//     Point3f get_high() const{return high;}
//     void set_low(const Point3f& l) {low = l;}
//     void set_high(const Point3f& h) {high = h;}

// };

class Bounds3f{
private:
public:
    bool flip_n;
    float radius;
    Point3f center;
    Bounds3f();
    Bounds3f(const bool& flip_n, const float& r, const Point3f& c);

    bool intersect_p( const Ray& r ) const;
    bool intersect( const Ray& r, float *t_hit, Surfel *sf ) const;

    virtual ~Bounds3f(){};
};

//-------------------------------------------------------------------------------
} // namespace rt3


#endif