#include "light.h"

namespace rt3{

bool VisibilityTester::unoccluded( const Scene& scene ) {

    Ray r{p0.p, p1.p - p0.p, 0.1, 1};
    Surfel isect;

    return (!scene.intersect(r, &isect));
}

} // namespace rt3