#include "sphere.h"

namespace rt3 {

bool Sphere::intersect_p( const Ray& r ) const{

    Vector3f d = r.get_direction();
    Vector3f oc = r.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * glm::dot(oc,oc) - radius * radius);

    return delta >= 0;
}

bool Sphere::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{
    Vector3f d = r.get_direction();
    Vector3f oc = r.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * glm::dot(oc,oc) - radius * radius);

    if(delta >= 0){
        real_type t1 = (-(glm::dot(oc, d)) - sqrt(delta)) / glm::dot(d,d); // First root
        real_type t2 = (-(glm::dot(oc, d)) + sqrt(delta)) / glm::dot(d,d); // Second root

        // Check if t1 or t2 are between the range
        if(r.get_t_min() < t1 && t1 < r.get_t_max()){
            *t_hit = t1;

            // Update the contact point
            sf->p = r(t1);
            // Update the surface normal (which is normally normalized)
            sf->n = glm::normalize(r(t1) - center);
            sf->wo = glm::normalize(-r.get_direction());
            return true;
        }
        else if(r.get_t_min() < t2 && t2 < r.get_t_max()){
            *t_hit = t2;

            // Update the contact point
            sf->p = r(t2);
            sf->wo = glm::normalize(-r.get_direction());
            // Update the surface normal (which is normally normalized)
            sf->n = glm::normalize(r(t2) - center);
            return true;
        }
    }

    return false;

}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Sphere object.
Sphere* create_sphere(const ParamSet &ps){
    
    real_type radius = retrieve(ps, "radius", real_type{0.5});
    Point3f center = retrieve(ps, "center", Point3f{0,0,0});

    // TODO: Add flip_normals
    return new Sphere(false, radius, center);
}


}  // namespace rt3
