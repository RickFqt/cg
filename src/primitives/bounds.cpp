#include "bounds.h"

namespace rt3 {

Bounds3f::Bounds3f(const Bounds3f & b1, const Bounds3f& b2){
    real_type min_x, min_y, min_z, max_x, max_y, max_z;
    min_x = std::min(b1.get_p_min().x, b2.get_p_min().x);
    min_y = std::min(b1.get_p_min().y, b2.get_p_min().y);
    min_z = std::min(b1.get_p_min().z, b2.get_p_min().z);

    max_x = std::max(b1.get_p_max().x, b2.get_p_max().x);
    max_y = std::max(b1.get_p_max().y, b2.get_p_max().y);
    max_z = std::max(b1.get_p_max().z, b2.get_p_max().z);

    p_min = {min_x, min_y, min_z};
    p_max = {max_x, max_y, max_z};
}

int Bounds3f::largest_extent(){
    Vector3f retorno = {0,0,0};
    for (int axis = 0; axis < 3; axis++){
        retorno[axis] = p_max[axis] - p_min[axis];
    }
    if(retorno[0] >= retorno[1] && retorno[0] >= retorno[2])return 0;
    if(retorno[1] >= retorno[0] && retorno[1] >= retorno[2])return 1;
    return 2;
}

bool Bounds3f::intersect_p( const Ray &ray, float &hit1, float &hit2 ) {

    const Point3f& ray_orig = ray.get_origin();
    const Vector3f&   ray_dir  = ray.get_direction();

    for (int axis = 0; axis < 3; axis++) {
        // const interval& ax = axis_interval(axis);
        const float  ax1 = p_min[axis];
        const float  ax2 = p_max[axis];
        const double adinv = 1.0 / ray_dir[axis];

        auto t0 = (ax1 - ray_orig[axis]) * adinv;
        auto t1 = (ax2 - ray_orig[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > hit1) hit1 = t0;
            if (t1 < hit2) hit2 = t1;
        } else {
            if (t1 > hit1) hit1 = t1;
            if (t0 < hit2) hit2 = t0;
        }

        if (hit2 <= hit1)
            return false;
    }
    return true;
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