#include "sphere.h"

namespace rt3 {

bool Sphere::intersect_p( const Ray& r ) const{

    Vector3f d = r.get_direction();
    Vector3f oc = r.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * glm::dot(oc,oc) - radius * radius);

    if(delta >= 0){
        real_type t = (-(glm::dot(oc, d)) - sqrt(delta)) / glm::dot(d,d);

        if(t < r.get_t_max()){
            r.set_t_max(t);
        }
    }

    return delta >= 0;
}

bool Sphere::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{
    // TODO: Update surfel and t_hit
    Vector3f d = r.get_direction();
    Vector3f oc = r.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * glm::dot(oc,oc) - radius * radius);

    if(delta >= 0){
        real_type t = (-(glm::dot(oc, d)) - sqrt(delta)) / glm::dot(d,d);

        if(t < r.get_t_max()){
            r.set_t_max(t);
        }
    }

    return delta >= 0;

}

/*
// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Sphere object.
Sphere* create_sphere(const ParamSet &ps){
    
    real_type radius = retrieve(ps, "radius", real_type{0.5});
    Point3f center = retrieve(ps, "center", Point3f{0,0,0});

    return new Sphere(radius, center);
}
*/


}  // namespace rt3
