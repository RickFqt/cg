#include "light.h"

namespace rt3{
constexpr float origin() {return 1.0f / 32.0f;}
constexpr float float_scale() {return 1.0f / 65536.0f;}
constexpr float int_scale() {return 256.0f;}
Vector3f offset_ray(const Vector3f p, const Vector3f n){
    Vector3i of_i (int_scale() * n);
    Vector3f p_i (
        (float)((int)(p.x)+((p.x < 0) ? -of_i.x : of_i.x)),
        (float)((int)(p.y)+((p.y < 0) ? -of_i.y : of_i.y)),
        (float)((int)(p.z)+((p.z < 0) ? -of_i.z : of_i.z))
    );

    return Vector3f(fabsf(p.x)<origin() ? p.x+float_scale()*n.x:p_i.x,
                    fabsf(p.y)<origin() ? p.y+float_scale()*n.y:p_i.y,
                    fabsf(p.z)<origin() ? p.z+float_scale()*n.z:p_i.z);
}

bool VisibilityTester::unoccluded( const Scene& scene, const Vector3f& n) {
    auto x = offset_ray(p0.p,n);

    Ray r{x, p1.p - x.p, 0, 1};
    Surfel isect;

    return (!scene.intersect(r, &isect));
}

} // namespace rt3