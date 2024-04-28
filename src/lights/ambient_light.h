#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H 1

#include "light.h"

namespace rt3 {

class AmbientLight : public Light {
private:
    Spectrum L;
public:
    virtual ~AmbientLight(){};
    AmbientLight(const Spectrum AL): Light((int)light_flag_e::ambient), L{AL}{ /* empty */ };

    Color24 sample_Li( const Surfel&   hit  /*in*/,
                             Vector3f        *wi  /*out*/,
                             VisibilityTester *vis /*out*/ ){return {0,0,0};}

    Spectrum get_L() const{return L;}
};

AmbientLight* create_ambient_light(const ParamSet& ps);

} // namespace rt3

#endif