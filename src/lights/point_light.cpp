#include "point_light.h"

namespace rt3 {

Spectrum PointLight::sample_Li( const Surfel& hit /*in*/, Vector3f *wi /*out*/, VisibilityTester *vis /*out*/ ){

    // Outgoing direction of ligh (l)
    *wi = glm::normalize(origin - hit.p);

    Surfel light_surfel;
    light_surfel.p = origin;
    *vis = VisibilityTester(hit, light_surfel);

    return I;
}

PointLight* create_point_light(const ParamSet& ps) {
    Spectrum I = retrieve(ps, "I", Spectrum{0.1, 0.1, 0.1});
    Point3f origin = retrieve(ps, "from", Point3f{0, 0, 0});
    return new PointLight(I, origin);
}

} // namespace rt3