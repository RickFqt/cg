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
    BlinnPhongMaterial * fm = dynamic_cast< BlinnPhongMaterial *>( isect.primitive->get_material() );

	// [4] INITIALIZE COMMON VARIABLES FOR BLINNPHONG INTEGRATOR (COEFFICIENTS AND VECTORS L, N, V, ETC.)
    Spectrum ka = fm->get_ka();
    Spectrum kd = fm->get_kd();
    Spectrum ks = fm->get_ks();
    real_type glossiness = fm->get_glossiness();
    Vector3f l; // light direction
    Vector3f n = isect.n; // surface normal
    Vector3f v = isect.wo; // view vector
    Vector3f h;
    VisibilityTester vis;
    real_type n_dot_l;
    real_type n_dot_h;
    Spectrum L_spectrum = {0,0,0};
    Spectrum L_curr_spectrum = {0,0,0};
    
	// [5] CALCULATE & ADD CONTRIBUTION FROM EACH LIGHT SOURCE
    for(auto light : scene.lights){
        // std::cout << "alface 1" << std::endl;
        if(light->flags == light_flag_e::ambient){
            std::shared_ptr<AmbientLight> al = std::dynamic_pointer_cast< AmbientLight >( light );
            // std::cout << "alface 2 " << al->get_L()[0] << " " << ka[0] << std::endl;
            L_spectrum[0] += (al->get_L()[0] * ka[0]);
            L_spectrum[1] += (al->get_L()[1] * ka[1]);
            L_spectrum[2] += (al->get_L()[2] * ka[2]);
            continue;
        }

        L_curr_spectrum = light->sample_Li(isect, &l, &vis);

        if(vis.unoccluded(scene)){
            n_dot_l = std::max(0.F, glm::dot(n, l));

            h = glm::normalize(v + l);
            n_dot_h = std::max(0.F, glm::dot(n, h));
            n_dot_h = pow(n_dot_h,  glossiness);
            
            L_spectrum[0] += kd[0] * L_curr_spectrum[0] * n_dot_l + ks[0] * L_curr_spectrum[0] * n_dot_h;
            L_spectrum[1] += kd[1] * L_curr_spectrum[1] * n_dot_l + ks[1] * L_curr_spectrum[1] * n_dot_h;
            L_spectrum[2] += kd[2] * L_curr_spectrum[2] * n_dot_l + ks[2] * L_curr_spectrum[2] * n_dot_h;
        }

    }
	// [6] ADD AMBIENT CONTRIBUTION HERE (if there is any).
	// [7] ADD MIRROR REFLECTION CONTRIBUTION

    // [8] Transform Spectrum to Color
    L[0] = std::min((int) (L_spectrum[0] * 255), 255);
    L[1] = std::min((int) (L_spectrum[1] * 255), 255);
    L[2] = std::min((int) (L_spectrum[2] * 255), 255);
    // std::cout << L_spectrum[0] << " " << L_spectrum[1] << " " << L_spectrum[2] << "\n";
	return L;
}

BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam){
    int depth = retrieve(ps, "depth", 1);
    return new BlinnPhongIntegrator(cam, depth);
}


} // namespace rt3