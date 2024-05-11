#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H 1

#include "light.h"

namespace rt3 {

class SpotLight : public Light {
private:
    Spectrum I;
    Point3f origin;
    Vector3f direction;
    real_type cutoff, faloff; // Angles (in radians)
public:
    virtual ~SpotLight(){};
    SpotLight(const Spectrum AL, const Point3f& o, const Vector3f& d, const real_type& c, const real_type& f ): 
    Light((int)light_flag_e::spot), I{AL}, origin{o}, direction{d}, cutoff{c}, faloff{f}{ /* empty */ };

    Spectrum sample_Li( const Surfel&   hit  /*in*/,
                             Vector3f        *wi  /*out*/,
                             VisibilityTester *vis /*out*/ );
};

SpotLight* create_spot_light(const ParamSet& ps);

} // namespace rt3

#endif