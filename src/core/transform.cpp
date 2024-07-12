#include "transform.h"

namespace rt3{

// ======================== =================================== =====================================
// ======================== Methods that transform other things =====================================
// ======================== =================================== =====================================

Point3f Transform::operator()(const Point3f &p, const int &vec_type) const {
    float x = p.x, y = p.y, z = p.z;
    if(vec_type == 0) { // É um point
        float xp = m[0][0]*x + m[0][1]*y + m[0][2]*z + m[0][3];
        float yp = m[1][0]*x + m[1][1]*y + m[1][2]*z + m[1][3];
        float zp = m[2][0]*x + m[2][1]*y + m[2][2]*z + m[2][3];
        return Point3f(xp, yp, zp);
    } 
    else if(vec_type == 1) { // É um vector
        return Vector3f(m[0][0]*x + m[0][1]*y + m[0][2]*z,
                        m[1][0]*x + m[1][1]*y + m[1][2]*z,
                        m[2][0]*x + m[2][1]*y + m[2][2]*z);
    } 
    else { // É uma normal
        return Normal3f(mInv[0][0]*x + mInv[1][0]*y + mInv[2][0]*z,
                        mInv[0][1]*x + mInv[1][1]*y + mInv[2][1]*z,
                        mInv[0][2]*x + mInv[1][2]*y + mInv[2][2]*z);
    }   
}

Ray Transform::operator()(const Ray &r) const { 
    Point3f o = (*this)(r.get_origin(), 0);
    Vector3f d = (*this)(r.get_direction(), 1);

    return Ray(o, d, r.get_t_min(), r.get_t_max());
}

Bounds3f Transform::operator()(const Bounds3f &b) const {
    const Transform &M = *this;

    Bounds3f ret( M( Point3f(b.get_p_min().x, b.get_p_min().y, b.get_p_min().z), 0 ) );   
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_min().y, b.get_p_min().z), 0));
    ret = Bounds3f(ret, M(Point3f(b.get_p_min().x, b.get_p_max().y, b.get_p_min().z), 0));
    ret = Bounds3f(ret, M(Point3f(b.get_p_min().x, b.get_p_min().y, b.get_p_max().z), 0));
    ret = Bounds3f(ret, M(Point3f(b.get_p_min().x, b.get_p_max().y, b.get_p_max().z), 0));
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_max().y, b.get_p_min().z), 0));
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_min().y, b.get_p_max().z), 0));
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_max().y, b.get_p_max().z), 0));
    return ret;
}

Surfel Transform::operator()(const Surfel &si) const {
    Surfel ret;
    ret.p =  (*this)(si.p, 0);
    ret.wo = glm::normalize((*this)(si.wo, 1));
    ret.n =  glm::normalize((*this)(si.n, 2));
    // TODO: uv coordnates
    ret.uv = si.uv;

    // TODO: Check if primitive is correct
    ret.primitive = si.primitive;

    return ret;
}


// ======================== =================================== =====================================
// ======================== =================================== =====================================
// ======================== =================================== =====================================

Transform Transform::operator*(const Transform &t2) const {

    return Transform( m * t2.getMatrix(), t2.getInverseMatrix() * mInv );

}


Transform Translate(const Vector3f &delta) {
    glm::mat4x4 m(1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z, 
                0, 0, 0,       1);
    glm::mat4x4 minv(1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z, 
                0, 0, 0,        1);
    return Transform(m, minv);
}

Transform Scale(float x, float y, float z) {
    glm::mat4x4 m(x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1);
    glm::mat4x4 minv(1.0/x,   0,   0, 0,
                    0,   1.0/y,   0, 0,
                    0,     0, 1.0/z, 0,
                    0,     0,   0, 1);
    return Transform(m, minv);
}

Transform Rotate(float theta, const Vector3f &axis) {
    Vector3f a = glm::normalize(axis);
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    glm::mat4x4 m;
    // <<Compute rotation of first basis vector>> 
    m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
    m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
    m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
    m[0][3] = 0;

    // <<Compute rotations of second and third basis vectors>> 
    m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
    m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
    m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
    m[1][3] = 0;
    
    m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
    m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
    m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
    m[2][3] = 0;

    // Fill the last line
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;

    return Transform(m);
}

Transform lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    glm::mat4x4 cameraToWorld;
    cameraToWorld[0][3] = pos.x;
    cameraToWorld[1][3] = pos.y;
    cameraToWorld[2][3] = pos.z;
    cameraToWorld[3][3] = 1;
    Vector3f dir = glm::normalize(look - pos);
    Vector3f right = glm::normalize(glm::cross(glm::normalize(up), dir));
    Vector3f newUp = glm::cross(dir, right);
    cameraToWorld[0][0] = right.x;
    cameraToWorld[1][0] = right.y;
    cameraToWorld[2][0] = right.z;
    cameraToWorld[3][0] = 0.;
    cameraToWorld[0][1] = newUp.x;
    cameraToWorld[1][1] = newUp.y;
    cameraToWorld[2][1] = newUp.z;
    cameraToWorld[3][1] = 0.;
    cameraToWorld[0][2] = dir.x;
    cameraToWorld[1][2] = dir.y;
    cameraToWorld[2][2] = dir.z;
    cameraToWorld[3][2] = 0.;

    return Transform(glm::inverse(cameraToWorld), cameraToWorld);
}


}
