#ifndef RAY_H
#define RAY_H 1

#include "rt3.h"

namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

class Ray {
public:
    Ray (const Point3f& o, const Vector3f& d,
        real_type start=0, real_type end=INFINITY) : o{o}, d{d},
        t_min{start}, t_max{end} {/*empty*/}
    Ray() : t_min{0.f}, t_max{INFINITY}{/*empty*/}

    friend std::ostream & operator<<( std::ostream & os_, const Ray & r_ ) {
        os_ << "o:[" << r_.o.x << " " << r_.o.y << " " << r_.o.z << "] ";
        os_ << "d:[" << r_.d.x << " " << r_.d.y << " " << r_.d.z << "]";
        return os_;
    }
    Point3f operator()(real_type t) const { return o + d * t; }

    Point3f get_origin()const{return o;}
    Vector3f get_direction()const{return d;}

    real_type get_t_max()const{return t_max;}
    real_type get_t_min()const{return t_min;}

    void set_t_min(const real_type& t)const{t_min = t;}
    void set_t_max(const real_type& t)const{t_max = t;}

private:
    Point3f o; //!< origin
    Vector3f d; //!< direction
    mutable real_type t_min, t_max; //!< parameters
    // ... the methods goes here
    Vector3f normalize(const Point3f& v) const;
};

//Example uses of Ray
//Ray r{ Point3f{0,0,0}, Vector3f{3,2,-4} }; // creating an infinity ray.
//float t{2.3};                            // a parameter for the ray.
//Point3f p = r( t );                       // overloading operator()(), so that it returns o + t*d.

//-------------------------------------------------------------------------------
} // namespace rt3

#endif