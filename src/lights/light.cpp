#include "light.h"

namespace rt3{
constexpr float origin() {return 1.0f / 32.0f;}
constexpr float float_scale() {return 1.0f / 65536.0f;}
constexpr float int_scale() {return 256.0f;}

float __int_as_float(int a)
{

  union {int a; float b;} u;

 u.a = a;

 return u.b;

}

int __float_as_int(float a)
{

  union {float a; int b;} u;

 u.a = a;

 return u.b;

}

Vector3f offset_ray(const Vector3f p, const Vector3f n){
    Vector3i of_i (int_scale() * n.x, int_scale() * n.y, int_scale() * n.z);
    Vector3f p_i (
        __int_as_float((__float_as_int(p.x))+((p.x < 0) ? -of_i.x : of_i.x)),
        __int_as_float((__float_as_int(p.y))+((p.y < 0) ? -of_i.y : of_i.y)),
        __int_as_float((__float_as_int(p.z))+((p.z < 0) ? -of_i.z : of_i.z))
    );

    return Vector3f(fabsf(p.x)<origin() ? p.x+float_scale()*n.x:p_i.x,
                    fabsf(p.y)<origin() ? p.y+float_scale()*n.y:p_i.y,
                    fabsf(p.z)<origin() ? p.z+float_scale()*n.z:p_i.z);
}

bool VisibilityTester::unoccluded( const Scene& scene, const Vector3f& n) {
    Point3f x = offset_ray(p0.p,(float)1000*n); // TODO: Why 1000?
    // Point3f x = p0.p + (float)0.001 * n; // Carlos Method

    Ray r{x, p1.p - x, 0, 1};
    Surfel isect;

    return (!scene.intersect_p(r));
}

} // namespace rt3