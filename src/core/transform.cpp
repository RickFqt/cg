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
    float wp = m[3][0]*x + m[3][1]*y + m[3][2]*z + m[3][3];
    if (wp == 1) return Point3f(xp, yp, zp);
    else         return Point3f(xp, yp, zp) / wp;
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

inline Ray Transform::operator()(const Ray &r) const { 
    // TODO: Ver esse erro ai
    // Vector3f oError;
    Point3f o = (*this)(r.get_origin(), 0 /*, &oError*/);
    Vector3f d = (*this)(r.get_direction(), 1);
    // <<Offset ray origin to edge of error bounds and compute tMax>> 
    // float lengthSquared = d.LengthSquared();
    // float tMax = r.tMax;
    // if (lengthSquared > 0) {
    //     float dt = glm::dot(Abs(d), oError) / lengthSquared;
    //     o += d * dt;
    //     tMax -= dt;
    // }

    return Ray(o, d, r.get_t_min(), r.get_t_max());
}

Bounds3f Transform::operator()(const Bounds3f &b) const {
    const Transform &M = *this;
    Bounds3f ret(M(Point3f(b.get_p_min().x, b.get_p_min().y, b.get_p_min().z)));    
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_min().y, b.get_p_min().z)));
    ret = Bounds3f(ret, M(Point3f(b.get_p_min().x, b.get_p_max().y, b.get_p_min().z)));
    ret = Bounds3f(ret, M(Point3f(b.get_p_min().x, b.get_p_min().y, b.get_p_max().z)));
    ret = Bounds3f(ret, M(Point3f(b.get_p_min().x, b.get_p_max().y, b.get_p_max().z)));
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_max().y, b.get_p_min().z)));
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_min().y, b.get_p_max().z)));
    ret = Bounds3f(ret, M(Point3f(b.get_p_max().x, b.get_p_max().y, b.get_p_max().z)));
    return ret;
}

Surfel Transform::operator()(const Surfel &si) const {
    Surfel ret;
    ret.p =  (*this)(si.p, 0);
    ret.wo = (*this)(si.wo, 1);
    ret.n =  (*this)(si.n, 2);
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

bool Transform::SwapsHandedness() const {
    float det = 
        m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
        m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
        m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    return det < 0;
}

bool Transform::HasScale() const {
    Vector3f v1 = (*this)(Vector3f(1, 0, 0), 1);
    Vector3f v2 = (*this)(Vector3f(0, 1, 0), 1);
    Vector3f v3 = (*this)(Vector3f(0, 0, 1), 1);
    real_type la2 = v1.x * v1.x + v1.y * v1.y + v1.z * v1.z;
    real_type lb2 = v2.x * v2.x + v2.y * v2.y + v2.z * v2.z;
    real_type lc2 = v3.x * v3.x + v3.y * v3.y + v3.z * v3.z;
#define NOT_ONE(x) ((x) < .999f || (x) > 1.001f)
    return (NOT_ONE(la2) || NOT_ONE(lb2) || NOT_ONE(lc2));
#undef NOT_ONE
}

Transform Transform::Translate(const Vector3f &delta) {
    glm::mat4 m(1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z, 
                0, 0, 0,       1);
    glm::mat4 minv(1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z, 
                0, 0, 0,        1);
    return Transform(m, minv);
}

Transform Transform::Scale(float x, float y, float z) {
    glm::mat4 m(x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1);
    glm::mat4 minv(1/x,   0,   0, 0,
                    0,   1/y,   0, 0,
                    0,     0, 1/z, 0,
                    0,     0,   0, 1);
    return Transform(m, minv);
}

Transform Transform::RotateX(float theta) {
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    glm::mat4 m(1,        0,         0, 0, 
                0, cosTheta, -sinTheta, 0,
                0, sinTheta,  cosTheta, 0,
                0,        0,         0, 1);
    return Transform(m, glm::transpose(m));
}

Transform Transform::RotateY(float theta) {
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    glm::mat4 m( cosTheta, 0, sinTheta, 0, 
                        0, 1,        0, 0,
                -sinTheta, 0, cosTheta, 0,
                        0, 0,        0, 1);
    return Transform(m, glm::transpose(m));
}


Transform Transform::RotateZ(float theta) {
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    glm::mat4 m(cosTheta, -sinTheta, 0, 0, 
                sinTheta,  cosTheta, 0, 0,
                       0,         0, 1, 0,
                       0,         0, 0, 1);
    return Transform(m, glm::transpose(m));
}

Transform Transform::lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    glm::mat4 cameraToWorld;
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
