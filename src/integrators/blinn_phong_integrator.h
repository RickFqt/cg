#ifndef BLINNPHONGINTEGRATOR_H
#define BLINNPHONGINTEGRATOR_H 1

#include "integrator.h"

namespace rt3 {

class BlinnPhongIntegrator : public SamplerIntegrator {
private:
int total_depth;

public:
    virtual ~BlinnPhongIntegrator(){};
    BlinnPhongIntegrator(std::shared_ptr<const Camera> cam, int depth):
    SamplerIntegrator(cam), total_depth{depth}{/* empty */}

    std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
    std::optional<Color24> Li( const Ray& ray, const Scene& scene, int depth ) const;
};

} // namespace rt3

#endif