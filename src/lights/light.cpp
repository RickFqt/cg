#include "light.h"

namespace rt3{

bool VisibilityTester::unoccluded( const Scene& scene ) {

    Ray r{p0.p, p1.p - p0.p, 0.1, 1};
    Surfel isect;
    scene.intersect(r, &isect);
    return r.get_t_max() > 0.98;
}

} // namespace rt3