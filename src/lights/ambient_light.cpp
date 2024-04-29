#include "ambient_light.h"

namespace rt3 {

AmbientLight* create_ambient_light(const ParamSet& ps) {
    Spectrum L = retrieve(ps, "L", Spectrum{0.1, 0.1, 0.1});
    return new AmbientLight(L);
}

} // namespace rt3