#include "blinn_phong_integrator.h"

namespace rt3 {

std::optional<Color24> BlinnPhongIntegrator::Li( const Ray& ray, const Scene& scene ) const{
    return Li( ray, scene, total_depth );
}

std::optional<Color24> BlinnPhongIntegrator::Li( const Ray &ray, const Scene &scene, int depth ) const
{
	// [0] FIRST STEP TO INITIATE `L` WITH THE COLOR VALUE TO BE RETURNED.
    Color24 L{0,0,0}; // The radiance
	// [1] FIND CLOSEST RAY INTERSECTION OR RETURN BACKGROUND RADIANCE
    Surfel isect; // Intersection information.
    if (!scene.intersect(ray, &isect)) {
        return {}; // empty far color.
    }
	// [2] SPECIAL SITUATION: IF THE RAY HITS THE SURFACE FROM "BEHIND" (INSIDE), WE DO NOT COLOR.

    // If cos between wo and n negative, the ray hits from behind
    if(glm::dot(isect.wo, isect.n) < 0) {
        // TODO: não sei
    }
	// [3] GET THE MATERIAL ASSOCIATED WITH THE HIT SURFACE
	// [4] INITIALIZE COMMON VARIABLES FOR BLINNPHONG INTEGRATOR (COEFFICIENTS AND VECTORS L, N, V, ETC.)
	// [5] CALCULATE & ADD CONTRIBUTION FROM EACH LIGHT SOURCE
	// [6] ADD AMBIENT CONTRIBUTION HERE (if there is any).
	// [7] ADD MIRROR REFLECTION CONTRIBUTION
	return L;
}

BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam){
    int depth = retrieve(ps, "depth", 1);
    return new BlinnPhongIntegrator(cam, depth);
}


} // namespace rt3