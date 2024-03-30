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
    real_type i = screen_window[Borders_e::l] + (screen_window[Borders_e::r] - screen_window[Borders_e::l])*(x + 0.5)/film->m_full_resolution[0];
    real_type j = screen_window[Borders_e::b] + (screen_window[Borders_e::t] - screen_window[Borders_e::b])*(y + 0.5)/film->m_full_resolution[1];

    Point3f ray_origin = e;
    Vector3f ray_direction = 1.F * w + i * u + j * v;
    // We can assume that focal distance is 1


    return Ray{ray_origin,ray_direction};
}

Ray OrthographicCamera::generate_ray(int x, int y){

    // Converts the (x,y) pixel coordinate to (i,j) coordinate in screen space
    real_type i = screen_window[Borders_e::l] + (screen_window[Borders_e::r] - screen_window[Borders_e::l])*(x + 0.5)/film->m_full_resolution[0];
    real_type j = screen_window[Borders_e::b] + (screen_window[Borders_e::t] - screen_window[Borders_e::b])*(y + 0.5)/film->m_full_resolution[1];

    Point3f ray_origin = e + i * u + j * v;
    Vector3f ray_direction = w;


    return Ray{ray_origin,ray_direction};
}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Film object.
PerspectiveCamera* create_perspective_camera(const ParamSet &cps, const ParamSet &lps, std::unique_ptr<Film>&& fml){
    // Retrieve look_up parameters
    Point3f look_from = retrieve(lps, "look_from", Point3f{0,0,0});
    Point3f look_at = retrieve(lps, "look_at", Point3f{0,0,0});
    Vector3f up = retrieve(lps, "up", Vector3f{0,0,0});
    
    vector<real_type> screen_window;

    if(cps.count("fovy") >= 1){
        real_type fovy = retrieve(cps, "fovy", 30);
        fovy = rt3::Radians(fovy);

        real_type H = tan(fovy/2);

        real_type proportion = (float)fml->m_full_resolution[0] / (float)fml->m_full_resolution[1];

        real_type left = -proportion*H;
        real_type right = proportion*H;
        real_type bottom = -H;
        real_type top = H;
        screen_window = {left, right, bottom, top};
    }
    else if(cps.count("screen_window") >= 1){
        screen_window = retrieve(cps, "screen_window", vector<real_type>{-1,1,-1,1});
    }
    else{
        // Default value for screen window.
        // By default, the values for this attribute is [−1,1] along the shorter image axis and
        // is set proportionally along the other (longer) axis.

        int x = fml->m_full_resolution[0];
        int y = fml->m_full_resolution[1];

        real_type proportion;

        if(x > y){
            proportion = (float)x/(float)y;
            screen_window = {-proportion, proportion, -1, 1};
        }
        else{
            proportion = (float)y/(float)x;
            screen_window = {-1, 1, -proportion, proportion};
        }

    }


    return new PerspectiveCamera(look_from, look_at, up, std::move(fml), screen_window);
}

OrthographicCamera* create_orthographic_camera(const ParamSet &cps, const ParamSet &lps, std::unique_ptr<Film>&& fml){

    // Retrieve look_up parameters
    Point3f look_from = retrieve(lps, "look_from", Point3f{0,0,0});
    Point3f look_at = retrieve(lps, "look_at", Point3f{0,0,0});
    Vector3f up = retrieve(lps, "up", Vector3f{0,0,0});
    vector<real_type> screen_window;

    if(cps.count("screen_window") >= 1){
        screen_window = retrieve(cps, "screen_window", vector<real_type>{-1,1,-1,1});
    }
    else{
        // Default value for screen window.
        // By default, the values for this attribute is [−1,1] along the shorter image axis and
        // is set proportionally along the other (longer) axis.

        int x = fml->m_full_resolution[0];
        int y = fml->m_full_resolution[1];

        real_type proportion;

        if(x > y){
            proportion = (float)x/(float)y;
            screen_window = {-proportion, proportion, -1, 1};
        }
        else{
            proportion = (float)y/(float)x;
            screen_window = {-1, 1, -proportion, proportion};
        }

    }

    return new OrthographicCamera(look_from, look_at, up, std::move(fml), screen_window);
}
}// namespace rt3