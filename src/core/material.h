#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt3.h"
#include "surfel.h"
#include "ray.h"

namespace rt3 {

// The Material describes how an object interacts with light.
class Material {
public:

    Material(){ /* empty */ };
    virtual ~Material(){ /* empty */ };

    virtual void scatter(Surfel *isect, Ray *r)=0;
};

// This concrete Material has only one information: a color.
class FlatMaterial : public Material {
private:

    Color24 kd;

public:

    FlatMaterial(Color24 kd): kd{kd}{ /* empty */ };
    virtual ~FlatMaterial(){ /* empty */ };

    // Returns the color
    Color24 get_kd() const{return kd;}

    void scatter(Surfel *isect, Ray *r){/* empty */};

};

}  // namespace rt3

#endif  // MATERIAL_H