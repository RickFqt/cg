#include "directional_light.h"

namespace rt3 {

Spectrum DirectionalLight::sample_Li( const Surfel& hit /*in*/, Vector3f *wi /*out*/, VisibilityTester *vis /*out*/ ){

    // Outgoing direction of light (l)
    // *wi = glm::normalize(origin - hit.p);


    Surfel light_surfel;
    // light_surfel.p = origin;
    *vis = VisibilityTester(hit, light_surfel);


    return L;
}

DirectionalLight* create_directional_light(const ParamSet& ps) {
    Spectrum L = retrieve(ps, "L", Spectrum{0.1, 0.1, 0.1});
    Point3f from = retrieve(ps, "from", Point3f{0, 0, 0});
    Point3f to = retrieve(ps, "to", Point3f{10, 10, 10});
    Vector3f direction = glm::normalize(to - from);

    return new DirectionalLight(L, direction);
}

} // namespace rt3