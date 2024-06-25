#ifndef BOUNDS_H
#define BOUNDS_H 1

#include "../core/rt3.h"
#include "../core/ray.h"
#include "../shapes/plane.h"
// #include "../shapes/sphere.h"

namespace rt3 {

class Bounds3f {
private:
    Point3f p_min, p_max; // Low represents the point with minimum, x, y and z. High the maximum x, y and z


public:
    Bounds3f(){}
    Bounds3f(const Point3f& l, const Point3f& h):
    p_min{l}, p_max{h}
    { /* empty */ } 
    Bounds3f(const Bounds3f & b1, const Bounds3f& b2);
	bool intersect_p(const Ray &ray, float *hit1, float *hit2) const;
    int largest_extent();

    float get_diagonal(){return glm::distance(p_min, p_max);};
    Point3f get_p_min() const{return p_min;}
    Point3f get_p_max() const{return p_max;}
    void set_p_min(const Point3f& l) {p_min = l;}
    void set_p_max(const Point3f& h) {p_max = h;}

};

// class Bounds3f{
// private:
// public:
//     bool flip_n;
//     float radius;
//     Point3f center;
//     Bounds3f();
//     Bounds3f(const bool& flip_n, const float& r, const Point3f& c);

//     bool intersect_p( const Ray& r ) const;
//     bool intersect( const Ray& r, float *t_hit, Surfel *sf ) const;

//     virtual ~Bounds3f(){};
// };

//-------------------------------------------------------------------------------
} // namespace rt3


#endif