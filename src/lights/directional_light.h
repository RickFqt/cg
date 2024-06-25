#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H 1

#include "light.h"
#include "../primitives/bounds.h"

namespace rt3 {

class DirectionalLight : public Light {
private:
    Spectrum L;
    Vector3f direction;
    real_type max_distance;
public:
    virtual ~DirectionalLight(){};
    DirectionalLight(const Spectrum AL, const Vector3f& d): 
    Light((int)light_flag_e::directional), L{AL}, direction{d}{ /* empty */ };

    Spectrum sample_Li( const Surfel&   hit  /*in*/,
                             Vector3f        *wi  /*out*/,
                             VisibilityTester *vis /*out*/ );

    virtual void preprocess( const Scene & );
};

DirectionalLight* create_directional_light(const ParamSet& ps);

} // namespace rt3

#endif