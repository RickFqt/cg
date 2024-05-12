#include "bounds.h"

namespace rt3 {

bool Bounds3f::intersect_p( const Ray& r ) const{

    // Cria os planos
    vector<Vector3f> vectors_low;
    vectors_low.push_back({1,0,0});
    vectors_low.push_back({0,1,0});
    vectors_low.push_back({0,0,1});
    vector<Vector3f> vectors_high;
    vectors_high.push_back({-1,0,0});
    vectors_high.push_back({0,-1,0});
    vectors_high.push_back({0,0,-1});
    
    Plane p1( vectors_low[0], vectors_low[1]);
    Surfel sf_temp;
    float t_temp;
    if(p1.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            return true;
        }
    }
    Plane p2(vectors_high[0], vectors_high[1]);
    if(p2.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            return true;
        }
    }
    Plane p3(vectors_high[1], vectors_high[2]);
    if(p3.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            return true;
        }
    }
    Plane p4(vectors_high[1], vectors_high[2]);
    if(p4.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            return true;
        }
    }
    Plane p5(vectors_high[1], vectors_high[2]);
    if(p5.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
            return true;
        }
    }
    Plane p6(vectors_high[1], vectors_high[2]);
    if(p6.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
            return true;
        }
    }

    return false;
}

bool Bounds3f::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{

    // Cria os planos
    vector<Vector3f> vectors_low;
    vectors_low.push_back({1,0,0});
    vectors_low.push_back({0,1,0});
    vectors_low.push_back({0,0,1});
    vector<Vector3f> vectors_high;
    vectors_high.push_back({-1,0,0});
    vectors_high.push_back({0,-1,0});
    vectors_high.push_back({0,0,-1});
    
    Plane p1( vectors_low[0], vectors_low[1]);
    Surfel sf_temp;
    float t_temp;
    if(p1.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            sf->p = sf_temp.p;
            sf->n = sf_temp.n;
            sf->wo = sf_temp.wo;
            *t_hit = t_temp;
            return true;
        }
    }
    Plane p2(vectors_high[0], vectors_high[1]);
    if(p2.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            sf->p = sf_temp.p;
            sf->n = sf_temp.n;
            sf->wo = sf_temp.wo;
            *t_hit = t_temp;
            return true;
        }
    }
    Plane p3(vectors_high[1], vectors_high[2]);
    if(p3.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            sf->p = sf_temp.p;
            sf->n = sf_temp.n;
            sf->wo = sf_temp.wo;
            *t_hit = t_temp;
            return true;
        }
    }
    Plane p4(vectors_high[1], vectors_high[2]);
    if(p4.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z)
        && (sf_temp.p.y >= low.y && sf_temp.p.y <= high.y) ){
            sf->p = sf_temp.p;
            sf->n = sf_temp.n;
            sf->wo = sf_temp.wo;
            *t_hit = t_temp;
            return true;
        }
    }
    Plane p5(vectors_high[1], vectors_high[2]);
    if(p5.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
            sf->p = sf_temp.p;
            sf->n = sf_temp.n;
            sf->wo = sf_temp.wo;
            *t_hit = t_temp;
            return true;
        }
    }
    Plane p6(vectors_high[1], vectors_high[2]);
    if(p6.intersect(r, &t_temp, &sf_temp)){   

        if( (sf_temp.p.x >= low.x && sf_temp.p.x <= high.x)
        && (sf_temp.p.z >= low.z && sf_temp.p.z <= high.z) ){
            sf->p = sf_temp.p;
            sf->n = sf_temp.n;
            sf->wo = sf_temp.wo;
            *t_hit = t_temp;
            return true;
        }
    }

    return false;
}


}