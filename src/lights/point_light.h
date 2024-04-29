#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H 1

#include "light.h"

namespace rt3 {

class PointLight : public Light {
private:
    Spectrum L;
    Point3f origin;
public:
    virtual ~PointLight(){};
    PointLight(const Spectrum AL, const Point3f& o): Light((int)light_flag_e::point), L{AL}, origin{o}{ /* empty */ };

    Spectrum sample_Li( const Surfel&   hit  /*in*/,
                             Vector3f        *wi  /*out*/,
                             VisibilityTester *vis /*out*/ );
};

PointLight* create_point_light(const ParamSet& ps);

} // namespace rt3

#endif