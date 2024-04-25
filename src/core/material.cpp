#include "material.h"

namespace rt3{

FlatMaterial* create_flat_material(const ParamSet& ps){
    
    Color24 color = retrieve(ps, "color", Color24{255,0,0});
    return new FlatMaterial(color);
}

}   // namespace rt3