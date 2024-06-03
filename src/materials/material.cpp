#include "material.h"

namespace rt3{

FlatMaterial* create_flat_material(const ParamSet& ps){
    
    Color24 color = retrieve(ps, "color", Color24{255,0,0});
    return new FlatMaterial(color);
}

BlinnPhongMaterial* create_blinn_phong_material(const ParamSet& ps){
    
    Color24 kaColor = retrieve(ps, "ambient", Color24{0,0,0});
    Color24 kdColor = retrieve(ps, "diffuse", Color24{0,0,0});
    Color24 ksColor = retrieve(ps, "specular", Color24{0,0,0});
    Spectrum kr = retrieve(ps, "mirror", Spectrum{0,0,0});
    real_type glossiness = retrieve(ps, "glossiness", 256.F);

    Spectrum ka;
    Spectrum kd;
    Spectrum ks;
    if(kaColor[0] > 1 || kaColor[1] > 1 || kaColor[2] > 1){
        // Transform Color to Spectrum
        ka[0] = std::min(((float)kaColor[0] / (float)255), 1.F);
        ka[1] = std::min(((float)kaColor[1] / (float)255), 1.F);
        ka[2] = std::min(((float)kaColor[2] / (float)255), 1.F);
    }
    else{
        ka[0] = (float)kaColor[0];
        ka[1] = (float)kaColor[1];
        ka[2] = (float)kaColor[2];
    }

    if(ksColor[0] > 1 || ksColor[1] > 1 || ksColor[2] > 1){
        // Transform Color to Spectrum
        ks[0] = std::min(((float)ksColor[0] / (float)255), 1.F);
        ks[1] = std::min(((float)ksColor[1] / (float)255), 1.F);
        ks[2] = std::min(((float)ksColor[2] / (float)255), 1.F);
    }
    else{
        ks[0] = (float)ksColor[0];
        ks[1] = (float)ksColor[1];
        ks[2] = (float)ksColor[2];
    }

    if(kdColor[0] > 1 || kdColor[1] > 1 || kdColor[2] > 1){
        // Transform Color to Spectrum
        kd[0] = std::min(((float)kdColor[0] / (float)255), 1.F);
        kd[1] = std::min(((float)kdColor[1] / (float)255), 1.F);
        kd[2] = std::min(((float)kdColor[2] / (float)255), 1.F);
    }
    else{
        kd[0] = (float)kdColor[0];
        kd[1] = (float)kdColor[1];
        kd[2] = (float)kdColor[2];
    }

    return new BlinnPhongMaterial(ka, kd, ks, kr, glossiness);
}

}   // namespace rt3