#include "camera.h"

namespace rt3{

using rt3::Vector3f;

Ray Camera::generate_ray(int x, int y){
    return Ray{Vector3f{0,0,0},Vector3f{0,0,0}};
}

Ray PerspectiveCamera::generate_ray(int x, int y){
    // TODO
    return Ray{Vector3f{0,0,0},Vector3f{0,0,0}};
}

Ray OrthographicCamera::generate_ray(int x, int y){
    // TODO
    return Ray{Vector3f{0,0,0},Vector3f{0,0,0}};
}


}// namespace rt3