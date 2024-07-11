#ifndef TRANSFORM_H
#define TRANSFORM_H 1

#include "rt3.h"
#include "ray.h"
#include "../primitives/bounds.h"
#include "surfel.h"

namespace rt3 {

class Transform {
public:
    Transform() { 
        m =   glm::mat4x4(1.0);
        mInv = m;

        // std::cout << "Transform default constructor" << std::endl;
        // std::cout << "m: " << m[0][0] << " " << m[0][1] << " " << m[0][2] << " " << m[0][3] << std::endl;
        // std::cout << "    " << m[1][0] << " " << m[1][1] << " " << m[1][2] << " " << m[1][3] << std::endl;
        // std::cout << "    " << m[2][0] << " " << m[2][1] << " " << m[2][2] << " " << m[2][3] << std::endl;
        // std::cout << "    " << m[3][0] << " " << m[3][1] << " " << m[3][2] << " " << m[3][3] << std::endl;
        // std::cout << "mInv: " << mInv[0][0] << " " << mInv[0][1] << " " << mInv[0][2] << " " << mInv[0][3] << std::endl;
        // std::cout << "    " << mInv[1][0] << " " << mInv[1][1] << " " << mInv[1][2] << " " << mInv[1][3] << std::endl;
        // std::cout << "    " << mInv[2][0] << " " << mInv[2][1] << " " << mInv[2][2] << " " << mInv[2][3] << std::endl;
        // std::cout << "    " << mInv[3][0] << " " << mInv[3][1] << " " << mInv[3][2] << " " << mInv[3][3] << std::endl;
        
    }
    Transform(const real_type mat[4][4]) {
        m = glm::mat4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                        mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = glm::inverse(m);
    }
    Transform(const glm::mat4x4 &m) : m(m), mInv(glm::inverse(m)) { }
    Transform(const glm::mat4x4 &m, const glm::mat4x4 &mInv) 
        : m(m), mInv(mInv) {
    }
    // void Print(FILE *f) const;
    friend Transform Inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }
    friend Transform Transpose(const Transform &t) {
        return Transform(glm::transpose(t.m), glm::transpose(t.mInv));
    }
    bool operator==(const Transform &t) const {
        return t.m == m && t.mInv == mInv;
    }
    bool operator!=(const Transform &t) const {
        return t.m != m || t.mInv != mInv;
    }
    bool operator<(const Transform &t2) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                if (m[i][j] < t2.m[i][j]) return true;
                if (m[i][j] > t2.m[i][j]) return false;
            }
        return false;
    }
    bool isIdentity() const {
        return (m == glm::mat4x4(1.0));
    }
    const glm::mat4x4 &getMatrix() const { return m; }
    const glm::mat4x4 &getInverseMatrix() const { return mInv; }

    Point3f operator()(const Point3f &p, const int &vec_type) const; // TODO: Investigar como é que isso funciona e ajeitar o retorno

    Ray operator()(const Ray &r) const;
    Bounds3f operator()(const Bounds3f &b) const;
    Surfel operator()(const Surfel &si) const;


    Transform operator*(const Transform &t2) const;


private:
    glm::mat4x4 m, mInv;

};

Transform Translate(const Vector3f &delta);

Transform Scale(float x, float y, float z);

Transform Rotate(float theta, const Vector3f &axis);

Transform lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);

} // namespace rt3

#endif