#include "material.h"

namespace rt3{

FlatMaterial* create_flat_material(const ParamSet& ps){
    
    Color24 color = retrieve(ps, "color", Color24{255,0,0});
    return new FlatMaterial(color);
}

BlinnPhongMaterial* create_blinn_phong_material(const ParamSet& ps){
    
    Spectrum ka = retrieve(ps, "ambient", Spectrum{0,0,0});
    Spectrum kd = retrieve(ps, "diffuse", Spectrum{0,0,0});
    Spectrum ks = retrieve(ps, "specular", Spectrum{0,0,0});
    real_type glossiness = retrieve(ps, "glossiness", 256);

    return new BlinnPhongMaterial(ka, kd, ks, glossiness);
}

}   // namespace rt3