#include "spot_light.h"

namespace rt3 {

Spectrum SpotLight::sample_Li( const Surfel& hit /*in*/, Vector3f *wi /*out*/, VisibilityTester *vis /*out*/ ){

    // Outgoing direction of light (l)
    *wi = glm::normalize(origin - hit.p);


    Surfel light_surfel;
    light_surfel.p = origin;
    *vis = VisibilityTester(hit, light_surfel);


    float cos_wi_direction = glm::dot(glm::normalize(-*wi), glm::normalize(direction));
    float result_angle = acos(cos_wi_direction);

    float coef = 0;


    if(result_angle <= faloff){
        coef = 1;
    }
    else if(result_angle <= cutoff){
        coef = 1 - (result_angle - faloff)/(cutoff - faloff);
        // std::cout << "Ta entre!! coef = " << coef << " , cutoff = " << cutoff << " , falloff = " << faloff << " , result angle= " << result_angle << "\n";
    }


    return (I * coef);
}

SpotLight* create_spot_light(const ParamSet& ps) {
    Spectrum I = retrieve(ps, "I", Spectrum{0.1, 0.1, 0.1});
    Point3f origin = retrieve(ps, "from", Point3f{0, 0, 0});
    Point3f to = retrieve(ps, "to", Point3f{10, 10, 10});
    Vector3f direction = glm::normalize(to - origin);

    real_type cutoff = retrieve(ps, "cutoff", 30.F);
    // cutoff = cutoff/2;
    cutoff = rt3::Radians(cutoff);
    real_type faloff = retrieve(ps, "falloff", 15.F);
    // faloff = faloff/2;
    faloff = rt3::Radians(faloff);
    return new SpotLight(I, origin, direction, cutoff, faloff);
}

} // namespace rt3