#ifndef MATERIAL_H
#define MATERIAL_H

#include "../core/rt3.h"
#include "../core/surfel.h"
#include "../core/ray.h"
#include "../core/paramset.h"

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

class BlinnPhongMaterial : public Material {
private:
    Spectrum ka; // Ambient coefficient
    Spectrum kd; // Diffuse coefficient
    Spectrum ks; // Specular coefficient
    real_type glossiness; // Glossiness exponent
public:

    BlinnPhongMaterial(const Spectrum& ka, const Spectrum& kd, const Spectrum& ks, const real_type& g):
    ka{ka}, kd{kd}, ks{ks}, glossiness{g}{ /* empty */ };
    virtual ~BlinnPhongMaterial(){ /* empty */ };

    Spectrum get_ka() const{return ka;}
    Spectrum get_kd() const{return kd;}
    Spectrum get_ks() const{return ks;}
    real_type get_glossiness() const{return glossiness;}

    void scatter(Surfel *isect, Ray *r){/* empty */};
};

// factory pattern functions.
FlatMaterial* create_flat_material(const ParamSet& ps);

BlinnPhongMaterial* create_blinn_phong_material(const ParamSet& ps);

}  // namespace rt3

#endif  // MATERIAL_H