#include "sphere.h"

namespace rt3 {

Sphere::Sphere(const bool& flip_n, const float& r, const Point3f& c, const Transform & obj_to_world):
Shape(flip_n, obj_to_world), radius{r}, center{c}
{}

bool Sphere::intersect_p( const Ray& r ) const{

    // std::cout << "Antes do transform: " << r << std::endl;
    Ray transformed_ray = (* world_to_obj)(r);
    // std::cout << "Depois do transform: " << transformed_ray << std::endl;

    Vector3f d = transformed_ray.get_direction();
    Vector3f oc = transformed_ray.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * (glm::dot(oc,oc) - radius * radius));

    if(delta >= 0){
        real_type t1 = (-(glm::dot(oc, d)) - sqrt(delta)) / glm::dot(d,d); // First root
        real_type t2 = (-(glm::dot(oc, d)) + sqrt(delta)) / glm::dot(d,d); // Second root

        // Check if t1 or t2 are between the range
        if(transformed_ray.get_t_min() < t1 && t1 < transformed_ray.get_t_max()){
            return true;
        }
        else if(transformed_ray.get_t_min() < t2 && t2 < transformed_ray.get_t_max()){
            return true;
        }
    }

    return false;
}

bool Sphere::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{

    // std::cout << "Qual a transformacao world to obj: " << std::endl;
    // auto m = world_to_obj->getMatrix();
    // for(int i = 0; i < 4; i++){
    //     for(int j = 0; j < 4; j++){
    //         std::cout << m[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // m = obj_to_world->getMatrix();
    // for(int i = 0; i < 4; i++){
    //     for(int j = 0; j < 4; j++){
    //         std::cout << m[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    Ray transformed_ray = (* world_to_obj)(r);

    Vector3f d = transformed_ray.get_direction();
    Vector3f oc = transformed_ray.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * (glm::dot(oc,oc) - radius * radius));

    if(delta >= 0){
        real_type t1 = (-(glm::dot(oc, d)) - sqrt(delta)) / glm::dot(d,d); // First root
        real_type t2 = (-(glm::dot(oc, d)) + sqrt(delta)) / glm::dot(d,d); // Second root

        // Check if t1 or t2 are between the range
        if(transformed_ray.get_t_min() < t1 && t1 < transformed_ray.get_t_max()){
            *t_hit = t1;

            // Update the contact point
            sf->p = transformed_ray(t1);
            // Update the surface normal (which is normally normalized)
            sf->n = glm::normalize(transformed_ray(t1) - center);
            sf->wo = glm::normalize(-transformed_ray.get_direction());

            // Transform the surface point to world coordinates
            *sf = (* obj_to_world)(*sf);

            return true;
        }
        else if(transformed_ray.get_t_min() < t2 && t2 < transformed_ray.get_t_max()){
            *t_hit = t2;

            // Update the contact point
            sf->p = transformed_ray(t2);
            sf->wo = glm::normalize(-transformed_ray.get_direction());
            // Update the surface normal (which is normally normalized)
            sf->n = glm::normalize(transformed_ray(t2) - center);

            // Transform the surface point to world coordinates
            *sf = (* obj_to_world)(*sf);

            return true;
        }
    }

    return false;

}

Bounds3f Sphere::world_bounds(){

    // std::cout << "batata" << std::endl;
    // std::cout << "batata2" << std::endl;

    return (* obj_to_world)(Bounds3f(center - radius - 1, center + radius + 1));
}

// Bounds3f Sphere::world_bounds(){

//     return Bounds3f(false, radius + 2, center);
// }

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Sphere object.
Sphere* create_sphere(const ParamSet &ps, const Transform & obj2world){
    
    real_type radius = retrieve(ps, "radius", real_type{0.5});
    Point3f center = retrieve(ps, "center", Point3f{0,0,0});

    // TODO: Add flip_normals
    return new Sphere(false, radius, center, obj2world);
}


}  // namespace rt3
