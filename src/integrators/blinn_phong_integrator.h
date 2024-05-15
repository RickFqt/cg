#ifndef BLINNPHONGINTEGRATOR_H
#define BLINNPHONGINTEGRATOR_H 1

#include "integrator.h"
#include "../lights/lights.h"

namespace rt3 {

class BlinnPhongIntegrator : public SamplerIntegrator {
private:
int max_depth;

public:
    virtual ~BlinnPhongIntegrator(){};
    BlinnPhongIntegrator(std::shared_ptr<const Camera> cam, int depth):
    SamplerIntegrator(cam), max_depth{depth}{/* empty */}

    std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
    std::optional<Spectrum> Li( const Ray& ray, const Scene& scene, int depth ) const;
};

BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam);

} // namespace rt3

#endif