#include "light.h"

namespace rt3{

bool VisibilityTester::unoccluded( const Scene& scene ) {

    Ray r{p0.p, p1.p - p0.p, 0.01, 1};
    Surfel isect;
    // scene.intersect(r, &isect);
    return (!scene.intersect(r, &isect));
}

} // namespace rt3