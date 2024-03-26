#ifndef PARSER_H
#define PARSER_H 1

#include "./rt3.h"

namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

class Ray {
    public:
        Ray (const Point3f& o, const Vector3f& d,
            real_type start=0, real_type end=INFINITY) : o{o}, d{d},
            t_min{start}, t_max{end} {std::cout << "a\n";}
        Ray() : t_min{0.f}, t_max{INFINITY}{std::cout << "b\n";}
    private:
        Point3f o; //!< origin
        Vector3f d; //!< direction
        mutable real_type t_min, t_max; //!< parameters
        // ... the methods goes here
        Vector3f normalize(const Point3f& v) const;
        Point3f operator()(real_type t) const { return o + d * t; }
};

//Example uses of Ray
//Ray r{ Point3f{0,0,0}, Vector3f{3,2,-4} }; // creating an infinity ray.
//float t{2.3};                            // a parameter for the ray.
//Point3f p = r( t );                       // overloading operator()(), so that it returns o + t*d.

//-------------------------------------------------------------------------------
} // namespace rt3

#endif