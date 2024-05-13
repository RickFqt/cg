#include "directional_light.h"

namespace rt3 {

Spectrum DirectionalLight::sample_Li( const Surfel& hit /*in*/, Vector3f *wi /*out*/, VisibilityTester *vis /*out*/ ){

    // Outgoing direction of light (l)
    // We need to find the origin of the ray
    Ray r(hit.p, -direction);
    Surfel hit_bounds;
    float t;
    Point3f origin = {0,0,0};

    if(world_bounds.intersect(r, &t, &hit_bounds)){
        origin = hit_bounds.p;
    }
    // else{
    //     std::cout << "Attention! World Bounds not found by Directional Light!\n";
    //     std::cout << "GLobal Bounds: " << 
    //     "{"<< world_bounds.get_low().x<<","<<world_bounds.get_low().y<<","<< world_bounds.get_low().z<<"}, " <<
    //     "{"<< world_bounds.get_high().x<<","<<world_bounds.get_high().y<<","<< world_bounds.get_high().z<<"}\n\n";
    // }
    *wi = glm::normalize(origin - hit.p);



    Surfel light_surfel;
    light_surfel.p = origin;
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