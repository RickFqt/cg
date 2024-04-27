#ifndef CAMERA_H
#define CAMERA_H 1

#include "../core/rt3.h"
#include "../core/ray.h"
#include "film.h"

namespace rt3{

// The role of the camera in a RT is to generate the rays that must interact with the scene.
class Camera{
    public:
    enum Borders_e {
        l = 0,  //!< Bottom left corner.
        r,      //!< Top left corner.
        b,      //!< Top right corner.
        t       //!< Bottom right corner.
    };

    // Camera Frame: [e, (u,v,w)]
    Point3f e; //!< Origin (look_from)
    Vector3f u; //!< --> normalize( cross( vup, w ) )
    Vector3f v; //!< --> normalize( cross ( w, u ) )
    Vector3f w; //!< The gaze of the camera. --> normalize(look_at - look_from)

    std::vector<real_type> screen_window = {-1, 1, -1, 1}; //!< The dimensions of the screen space (left, right, bottom and top)
    std::unique_ptr<Film> film; //!< Represents an image generated by the ray tracer, stored in the camera

    Camera(Point3f look_from, Point3f look_at, Vector3f up, std::unique_ptr<Film>&& film, vector<real_type> screen_window):
    screen_window{screen_window},film{std::move(film)}
    {
        Vector3f gaze = look_at - look_from; w = glm::normalize(gaze); // left-hand orientation
        u = glm::normalize( glm::cross( up, w ) ); // The order inside cross matters.
        v = glm::normalize( glm::cross ( u, w ) );
        e = look_from;
    }
    virtual ~Camera(){ /* empty */ };

    virtual Ray generate_ray(int x, int y)const;
};

// The Perspective Camera shoots rays from a single point to every pixel area into the scene.
class PerspectiveCamera : public Camera {

    public:
	Ray generate_ray(int x, int y)const;

    PerspectiveCamera(Point3f look_from, Point3f look_at, Vector3f up, std::unique_ptr<Film>&& film, vector<real_type> screen_window):
    Camera(look_from, look_at, up, std::move(film), screen_window)
    {/*empty*/};
    virtual ~PerspectiveCamera(){ /* empty */ };
};

// The Orthographic Camera shoots rays from each pixel to a direction (usually) perpendicular to the image plane into the scene.
class OrthographicCamera : public Camera {

    public:
	Ray generate_ray(int x, int y)const;

    OrthographicCamera(Point3f look_from, Point3f look_at, Vector3f up, std::unique_ptr<Film>&& film, vector<real_type> screen_window):
    Camera(look_from, look_at, up, std::move(film), screen_window)
    {/*empty*/};

    virtual ~OrthographicCamera(){ /* empty */ };
};

//-------------------------------------------------------------------------------
// Factory pattern. It's not part of this class.
OrthographicCamera* create_orthographic_camera(const ParamSet &cps, const ParamSet &lps, std::unique_ptr<Film>&& fml);

// Factory pattern. It's not part of this class.
PerspectiveCamera* create_perspective_camera(const ParamSet &cps, const ParamSet &lps, std::unique_ptr<Film>&& fml);
} // namespace rt3

#endif