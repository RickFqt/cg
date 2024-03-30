#include "camera.h"

namespace rt3{

using rt3::Vector3f;

/**
 * @brief Generates a ray for a pixel (x, y) on the raster image.
 */
Ray Camera::generate_ray(int x, int y){
    return Ray{Vector3f{0,0,0},Vector3f{0,0,0}};
}

Ray PerspectiveCamera::generate_ray(int x, int y){

    // Converts the (x,y) pixel coordinate to (i,j) coordinate in screen space
    real_type i = screen_window[Borders_e::l] + (screen_window[Borders_e::r] - screen_window[Borders_e::l])*(x + 0.5)/film.m_full_resolution[0];
    real_type j = screen_window[Borders_e::b] + (screen_window[Borders_e::t] - screen_window[Borders_e::b])*(y + 0.5)/film.m_full_resolution[1];

    Point3f ray_origin = e;
    Vector3f ray_direction = 1.F * w + i * u + j * v;
    // We can assume that focal distance is 1


    return Ray{ray_origin,ray_direction};
}

Ray OrthographicCamera::generate_ray(int x, int y){

    // Converts the (x,y) pixel coordinate to (i,j) coordinate in screen space
    real_type i = screen_window[Borders_e::l] + (screen_window[Borders_e::r] - screen_window[Borders_e::l])*(x + 0.5)/film.m_full_resolution[0];
    real_type j = screen_window[Borders_e::b] + (screen_window[Borders_e::t] - screen_window[Borders_e::b])*(y + 0.5)/film.m_full_resolution[1];

    Point3f ray_origin = e + i * u + j * v;
    Vector3f ray_direction = w;


    return Ray{ray_origin,ray_direction};
}


}// namespace rt3