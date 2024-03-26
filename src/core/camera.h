#ifndef CAMERA_H
#define CAMERA_H 1

#include "rt3.h"
#include "ray.h"
#include "film.h"

namespace rt3{

class Camera{
    // TODO: Add members and methods here
    private:
    
    Point3f look_from; //!< Origin
    Point3f look_at;
    Vector3f gaze; //!< The direction of the camera's view (look_at - look_from)
    Vector3f vup; //!< The up direction of the camera.

    Film film;

    virtual Ray generate_ray(int x, int y);

    public:
    // TODO: Ctro
    // Camera(){};
    virtual ~Camera(){ /* empty */ };
};


class PerspectiveCamera : public Camera {
    // TODO: Add members and methods here
    private:

    float fovy; //!< The angle of the camera's view
    public:
	Ray generate_ray(int x, int y);

    /// TODO: Ctro
    // PerspectiveCamera(){};
    virtual ~PerspectiveCamera(){ /* empty */ };
};


class OrthographicCamera : public Camera {
    // TODO: Add members and methods here
    public:
	Ray generate_ray(int x, int y);

    /// TODO: Ctro
    // OrthographicCamera(){};

    virtual ~OrthographicCamera(){ /* empty */ };
};

//-------------------------------------------------------------------------------
} // namespace rt3

#endif