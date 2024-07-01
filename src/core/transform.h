#ifndef TRANSFORM_H
#define TRANSFORM_H 1

#include "rt3.h"
#include "ray.h"

namespace rt3 {

class Transform {
public:
    Transform() { 
        m =   glm::mat4(1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1);
        mInv = glm::inverse(m);
    }
    Transform(const real_type mat[4][4]) {
        m = glm::mat4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                        mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                        mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                        mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = glm::inverse(m);
    }
    Transform(const glm::mat4 &m) : m(m), mInv(glm::inverse(m)) { }
    Transform(const glm::mat4 &m, const glm::mat4 &mInv) 
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
        return (m[0][0] == 1.f && m[0][1] == 0.f &&
                m[0][2] == 0.f && m[0][3] == 0.f &&
                m[1][0] == 0.f && m[1][1] == 1.f &&
                m[1][2] == 0.f && m[1][3] == 0.f &&
                m[2][0] == 0.f && m[2][1] == 0.f &&
                m[2][2] == 1.f && m[2][3] == 0.f &&
                m[3][0] == 0.f && m[3][1] == 0.f &&
                m[3][2] == 0.f && m[3][3] == 1.f);
    }
    const glm::mat4 &getMatrix() const { return m; }
    const glm::mat4 &getInverseMatrix() const { return mInv; }
    bool HasScale() const;
    Point3f operator()(const Point3f &p, const int &vec_type) const; // TODO: Investigar como é que isso funciona e ajeitar o retorno
    // Vector3f operator()(const Vector3f &v) const;
    // Normal3f operator()(const Normal3f &) const;
    // void operator()(const Normal3f &, Normal3f *nt) const;
    inline Ray operator()(const Ray &r) const;
    // inline RayDifferential operator()(const RayDifferential &r) const;
    Bounds3f operator()(const Bounds3f &b) const;
    Transform operator*(const Transform &t2) const;
    bool SwapsHandedness() const;
    // SurfaceInteraction operator()(const SurfaceInteraction &si) const;
    Point3f operator()(const Point3f &pt, Vector3f *absError) const;
    Point3f
    operator()(const Point3f &p, const Vector3f &pError,
                Vector3f *pTransError) const;
    // Vector3f operator()(const Vector3f &v, Vector3f *vTransError) const;
    // Vector3f operator()(const Vector3f &v, const Vector3f &vError, Vector3f *vTransError) const;
    inline Ray operator()(const Ray &r, Vector3f *oError,
        Vector3f *dError) const;
    inline Ray operator()(const Ray &r, const Vector3f &oErrorIn,
                            const Vector3f &dErrorIn, Vector3f *oErrorOut,
                            Vector3f *dErrorOut) const;

    Transform Translate(const Vector3f &delta);
    Transform Scale(float x, float y, float z);
    Transform RotateX(float theta);
    Transform RotateY(float theta);
    Transform RotateZ(float theta);
    Transform lookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);

private:
    glm::mat4 m, mInv;
    friend class AnimatedTransform;
    friend struct Quaternion;

};

} // namespace rt3

#endif