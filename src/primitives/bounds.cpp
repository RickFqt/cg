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
    
    Plane p1(false, vectors_low[0], vectors_low[1]);
    Surfel sf;
    float t;
    if(p1.intersect(r, &t, &sf)){   

        if( (sf.p.x >= low.x && sf.p.x <= high.x)
        && (sf.p.y >= low.y && sf.p.y <= high.y) ){
            return true;
        }
    }
    Plane p2(false,vectors_high[0], vectors_high[1]);
    Surfel sf;
    float t;
    if(p2.intersect(r, &t, &sf)){   

        if( (sf.p.x >= low.x && sf.p.x <= high.x)
        && (sf.p.y >= low.y && sf.p.y <= high.y) ){
            return true;
        }
    }
    Plane p3(false,vectors_high[1], vectors_high[2]);
    Surfel sf;
    float t;
    if(p3.intersect(r, &t, &sf)){   

        if( (sf.p.z >= low.z && sf.p.z <= high.z)
        && (sf.p.y >= low.y && sf.p.y <= high.y) ){
            return true;
        }
    }
    Plane p4(false,vectors_high[1], vectors_high[2]);
    Surfel sf;
    float t;
    if(p4.intersect(r, &t, &sf)){   

        if( (sf.p.z >= low.z && sf.p.z <= high.z)
        && (sf.p.y >= low.y && sf.p.y <= high.y) ){
            return true;
        }
    }
    Plane p5(false,vectors_high[1], vectors_high[2]);
    Surfel sf;
    float t;
    if(p5.intersect(r, &t, &sf)){   

        if( (sf.p.x >= low.x && sf.p.x <= high.x)
        && (sf.p.z >= low.z && sf.p.z <= high.z) ){
            return true;
        }
    }
    Plane p6(false,vectors_high[1], vectors_high[2]);
    Surfel sf;
    float t;
    if(p6.intersect(r, &t, &sf)){   

        if( (sf.p.x >= low.x && sf.p.x <= high.x)
        && (sf.p.z >= low.z && sf.p.z <= high.z) ){
            return true;
        }
    }

    return false;
}

bool Bounds3f::intersect( const Ray& r, Surfel *sf ) const{

    return true;
}


}