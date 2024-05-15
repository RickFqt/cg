#include "plane.h"

namespace rt3 {



bool Plane::intersect_p( const Ray& r ) const{

    Vector3f d = r.get_direction();
    Vector3f o = r.get_origin();
    
    float o1 = o[0];
    float o2 = o[1];
    float o3 = o[2];
    float d1 = d[0];
    float d2 = d[1];
    float d3 = d[2];
    float n1 = normal[0];
    float n2 = normal[1];
    float n3 = normal[2];
    float a1 = point[0];
    float a2 = point[1];
    float a3 = point[2];

    float numerador = n1*(a1 - o1) + n2*(a2 - o2) + n3*(a3 - o3);
    float denominador = d1 + d2 + d3;

    if(d1 + d2 + d3 != 0){
        return numerador/denominador > 0;
    }

    return false;
}

bool Plane::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{
    

    Vector3f d = r.get_direction();
    Vector3f o = r.get_origin();
    
    float o1 = o[0];
    float o2 = o[1];
    float o3 = o[2];
    float d1 = d[0];
    float d2 = d[1];
    float d3 = d[2];
    float n1 = normal[0];
    float n2 = normal[1];
    float n3 = normal[2];
    float a1 = point[0];
    float a2 = point[1];
    float a3 = point[2];

    float numerador = n1*(a1 - o1) + n2*(a2 - o2) + n3*(a3 - o3);
    float denominador = d1 + d2 + d3;
    float t;

    if(d1 + d2 + d3 != 0){
        t = numerador/denominador;

        if(0 <= t && t < r.get_t_max()){
            *t_hit = t;

            // Update the contact point
            sf->p = r(t);
            // Update the surface normal (which is normally normalized)
            sf->n = normal;
            sf->wo = glm::normalize(-r.get_direction());
            return true;
        }
    }

    return false;

}

// Bounds3f Plane::world_bounds(){

//     // TODO: Fix this
//     return Bounds3f({0,0,0}, {10,10,10});
// }


}  // namespace rt3
