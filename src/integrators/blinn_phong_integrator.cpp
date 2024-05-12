#include "blinn_phong_integrator.h"

namespace rt3 {

std::optional<Color24> BlinnPhongIntegrator::Li( const Ray& ray, const Scene& scene ) const{
    Color24 L{0,0,0};
    auto temp_L = Li( ray, scene, 0 );

    if(!temp_L.has_value()){
        return {};
    }
    else{
        // Transform Spectrum to Color
        L[0] = std::min((int) (temp_L.value()[0] * 255), 255);
        L[1] = std::min((int) (temp_L.value()[1] * 255), 255);
        L[2] = std::min((int) (temp_L.value()[2] * 255), 255);
    }
    return L;
}

std::optional<Spectrum> BlinnPhongIntegrator::Li( const Ray &ray, const Scene &scene, int depth ) const
{
	// [0] FIRST STEP TO INITIATE `L` WITH THE COLOR VALUE TO BE RETURNED.
    Spectrum L{0,0,0}; // The radiance
	// [1] FIND CLOSEST RAY INTERSECTION OR RETURN BACKGROUND RADIANCE
    Surfel isect; // Intersection information.
    if (!scene.intersect(ray, &isect)) {
        return {}; // empty far color.
    }
	// [2] SPECIAL SITUATION: IF THE RAY HITS THE SURFACE FROM "BEHIND" (INSIDE), WE DO NOT COLOR.

    // If cos between wo and n negative, the ray hits from behind
    if(glm::dot(glm::normalize(isect.wo), isect.n) < 0) {
        return {};
    }
	// [3] GET THE MATERIAL ASSOCIATED WITH THE HIT SURFACE
    BlinnPhongMaterial * fm = dynamic_cast< BlinnPhongMaterial *>( isect.primitive->get_material() );

	// [4] INITIALIZE COMMON VARIABLES FOR BLINNPHONG INTEGRATOR (COEFFICIENTS AND VECTORS L, N, V, ETC.)
    Spectrum ka = fm->get_ka();
    Spectrum kd = fm->get_kd();
    Spectrum ks = fm->get_ks();
    Spectrum km = fm->get_kr();
    real_type glossiness = fm->get_glossiness();
    Vector3f l; // light direction
    Vector3f n = isect.n; // surface normal
    Vector3f v = isect.wo; // view vector
    Vector3f h;
    VisibilityTester vis;
    real_type n_dot_l, n_dot_h;
    Spectrum L_curr = {0,0,0};
    
	// [5] CALCULATE & ADD CONTRIBUTION FROM EACH LIGHT SOURCE
    for(auto light : scene.lights){
        if(light->flags == light_flag_e::ambient){

            std::shared_ptr<AmbientLight> al = std::dynamic_pointer_cast< AmbientLight >( light );
            L[0] += (al->get_L()[0] * ka[0]);
            L[1] += (al->get_L()[1] * ka[1]);
            L[2] += (al->get_L()[2] * ka[2]);
            continue;
        }
        else if(light->flags == light_flag_e::directional){
            std::shared_ptr<DirectionalLight> dl = std::dynamic_pointer_cast< DirectionalLight >( light );
            dl->set_world_bounds(scene.get_aggregate()->world_bounds());
        }

        L_curr = light->sample_Li(isect, &l, &vis);

        if(vis.unoccluded(scene)){
            n_dot_l = std::max(0.F, glm::dot(n, l));

            h = glm::normalize(v + l);
            n_dot_h = std::max(0.F, glm::dot(n, h));
            n_dot_h = pow(n_dot_h,  glossiness);
            
            // std::cout << L_curr_spectrum[0] << " " << L_curr_spectrum[1] << " " << L_curr_spectrum[2] << "\n";
            // Add Diffuse and Specular contribution
            L[0] += (kd[0] * L_curr[0] * n_dot_l) + (ks[0] * L_curr[0] * n_dot_h);
            L[1] += (kd[1] * L_curr[1] * n_dot_l) + (ks[1] * L_curr[1] * n_dot_h);
            L[2] += (kd[2] * L_curr[2] * n_dot_l) + (ks[2] * L_curr[2] * n_dot_h);
        }

    }
	// [6] ADD AMBIENT CONTRIBUTION HERE (if there is any).

    

	// [7] ADD MIRROR REFLECTION CONTRIBUTION

    // [7.1] Find new ray, based on perfect reflection about surface normal.
    Point3f origin = isect.p;
    Vector3f reflected_direction = glm::normalize((ray.get_direction()) - 2*(glm::dot(ray.get_direction(),n))*n);
    Ray reflected_ray{origin, reflected_direction, 0.1};
    // [7.2] Offset reflect_ray by an epsilon, to avoid self-intersection caused by rounding error.



    // [7.3] Recursive call of Li() with new reflected ray.
    if ( depth < max_depth ){
        // std::cout << depth << "\n";
        // std::cout << max_depth << "\n";
        auto temp_L = BlinnPhongIntegrator::Li(reflected_ray, scene, depth+1);
        if(temp_L.has_value()){
            // std::cout <<  temp_S[0] << " " <<  temp_S[1] << " " <<  temp_S[2] << "\n";
            // std::cout << km[0] << " " << km[1] << " " << km[2] << "\n";
            L[0] = (L[0] + km[0] * temp_L.value()[0]);
            L[1] = (L[1] + km[1] * temp_L.value()[1]);
            L[2] = (L[2] + km[2] * temp_L.value()[2]);
        }
        // else{
        //     Point2i img_dim = camera->film->get_resolution(); // Retrieve the image dimensions in pixels.
        //     Point2f screen_coord{ float(120)/float(img_dim.x), float(200)/float(img_dim.y) };
        //     Color24 abacate = scene.background->sampleXYZ(screen_coord);
        //     Spectrum blau {(float)abacate[0]/(float)255, (float)abacate[1]/(float)255, (float)abacate[2]/(float)255};

        //     L = L + (km * blau);
        // }
    }

	return L;
}

BlinnPhongIntegrator* create_blinn_phong_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam){
    int depth = retrieve(ps, "depth", 1);
    std::cout << depth << " dwawafdwajhwafd" << "\n";
    return new BlinnPhongIntegrator(cam, depth);
}


} // namespace rt3