#include "ray.h"
#include "rt3.h"

namespace rt3 {

using rt3::Vector3f;

Vector3f Ray::normalize(const Vector3f& v) const{

    return glm::normalize(v);
}

//-------------------------------------------------------------------------------
} // namespace rt3