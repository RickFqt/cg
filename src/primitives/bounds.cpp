#include "bounds.h"

namespace rt3 {

Bounds3f::Bounds3f():flip_n{false}, radius{0}, center{0,0,0}{}

Bounds3f::Bounds3f(const bool& flip_n, const float& r, const Point3f& c):
    flip_n{flip_n}, radius{r}, center{c}
    {}

bool Bounds3f::intersect_p( const Ray& r ) const{

    Vector3f d = r.get_direction();
    Vector3f oc = r.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * (glm::dot(oc,oc) - radius * radius));

    return delta >= 0;
}

bool Bounds3f::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{
    Vector3f d = r.get_direction();
    Vector3f oc = r.get_origin() - center;
    float delta = glm::dot(oc, d) * glm::dot(oc, d) - ( glm::dot(d,d) * (glm::dot(oc,oc) - radius * radius));

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

// bool Bounds3f::intersect_p( const Ray& r ) const{

//     // Cria os planos
//     vector<Vector3f> vectors_low;
//     vectors_low.push_back({1,0,0});
//     vectors_low.push_back({0,1,0});
//     vectors_low.push_back({0,0,1});
//     vector<Vector3f> vectors_high;
//     vectors_high.push_back({-1,0,0});
//     vectors_high.push_back({0,-1,0});
//     vectors_high.push_back({0,0,-1});
    
//     Plane p1(true, vectors_low[0], vectors_low[1]);
//     Surfel sf_temp;
//     float t_temp;
//     Ray r1{r};
//     if(p1.intersect(r1, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             r.set_t_min(r1.get_t_min());
//             r.set_t_max(r1.get_t_max());
//             return true;
//         // }
//     }
//     Ray r2{r};
//     Plane p2(true,vectors_high[0], vectors_high[1]);
//     if(p2.intersect(r2, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             r.set_t_min(r2.get_t_min());
//             r.set_t_max(r2.get_t_max());
//             return true;
//         // }
//     }
//     Ray r3{r};
//     Plane p3(true,vectors_low[1], vectors_low[2]);
//     if(p3.intersect(r3, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             r.set_t_min(r3.get_t_min());
//             r.set_t_max(r3.get_t_max());
//             return true;
//         // }
//     }
//     Ray r4{r};
//     Plane p4(true,vectors_high[1], vectors_high[2]);
//     if(p4.intersect(r4, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             r.set_t_min(r4.get_t_min());
//             r.set_t_max(r4.get_t_max());
//             return true;
//         // }
//     }
//     Ray r5{r};
//     Plane p5(true,vectors_low[0], vectors_low[2]);
//     if(p5.intersect(r5, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
//             r.set_t_min(r5.get_t_min());
//             r.set_t_max(r5.get_t_max());
//             return true;
//         // }
//     }
//     Ray r6{r};
//     Plane p6(true,vectors_high[0], vectors_high[2]);
//     if(p6.intersect(r6, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
//             r.set_t_min(r6.get_t_min());
//             r.set_t_max(r6.get_t_max());
//             return true;
//         // }
//     }

//     return false;
// }

// bool Bounds3f::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{

//     // Cria os planos
//     vector<Vector3f> vectors_low;
//     vectors_low.push_back({1,0,0});
//     vectors_low.push_back({0,1,0});
//     vectors_low.push_back({0,0,1});
//     vector<Vector3f> vectors_high;
//     vectors_high.push_back({-1,0,0});
//     vectors_high.push_back({0,-1,0});
//     vectors_high.push_back({0,0,-1});
    
//     Plane p1(false,vectors_low[0], vectors_low[1]); // Aqui eh false eu acho
//     Surfel sf_temp;
//     float t_temp;
//     Ray r1{r};
//     if(p1.intersect(r1, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             sf->p = sf_temp.p;
//             sf->n = sf_temp.n;
//             sf->wo = sf_temp.wo;
//             *t_hit = t_temp;
//             r.set_t_min(r1.get_t_min());
//             r.set_t_max(r1.get_t_max());
//             return true;
//         // }
//     }
//     Plane p2(false,vectors_high[0], vectors_high[1]);
//     Ray r2{r};
//     if(p2.intersect(r2, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             sf->p = sf_temp.p;
//             sf->n = sf_temp.n;
//             sf->wo = sf_temp.wo;
//             *t_hit = t_temp;
//             r.set_t_min(r2.get_t_min());
//             r.set_t_max(r2.get_t_max());
//             return true;
//         // }
//     }
//     Plane p3(true,vectors_low[1], vectors_low[2]); // Aqui eh true eu acho
//     Ray r3{r};
//     if(p3.intersect(r3, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             sf->p = sf_temp.p;
//             sf->n = sf_temp.n;
//             sf->wo = sf_temp.wo;
//             *t_hit = t_temp;
//             r.set_t_min(r3.get_t_min());
//             r.set_t_max(r3.get_t_max());
//             return true;
//         // }
//     }
//     Plane p4(false,vectors_high[1], vectors_high[2]); // Eh false eu acho
//     Ray r4{r};
//     if(p4.intersect(r4, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
//         // && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
//             sf->p = sf_temp.p;
//             sf->n = sf_temp.n;
//             sf->wo = sf_temp.wo;
//             *t_hit = t_temp;
//             r.set_t_min(r4.get_t_min());
//             r.set_t_max(r4.get_t_max());
//             return true;
//         // }
//     }
//     Plane p5(true,vectors_low[0], vectors_low[2]); // Aqui eh true eu acho
//     Ray r5{r};
//     if(p5.intersect(r5, &t_temp, &sf_temp)){   

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
//             sf->p = sf_temp.p;
//             sf->n = sf_temp.n;
//             sf->wo = sf_temp.wo;
//             *t_hit = t_temp;
//             r.set_t_min(r5.get_t_min());
//             r.set_t_max(r5.get_t_max());
//             return true;
//         // }
//     }
//     Plane p6(false,vectors_high[0], vectors_high[2]); // Aqui eh false eu acho
//     Ray r6{r};
//     if(p6.intersect(r6, &t_temp, &sf_temp)){   

//         // std::cout << "Aaaaa\n";

//         // if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
//         // && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
//             sf->p = sf_temp.p;
//             sf->n = sf_temp.n;
//             sf->wo = sf_temp.wo;
//             *t_hit = t_temp;
//             r.set_t_min(r6.get_t_min());
//             r.set_t_max(r6.get_t_max());
//             return true;
//         // }
//     }

//     return false;
// }


}