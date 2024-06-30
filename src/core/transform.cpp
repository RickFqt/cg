#include "transform.h"

namespace rt3{

bool Transform::HasScale() const {
    Vector3f v1 = (*this)(Vector3f(1, 0, 0));
    Vector3f v2 = (*this)(Vector3f(0, 1, 0));
    Vector3f v3 = (*this)(Vector3f(0, 0, 1));
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


}
