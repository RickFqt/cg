#include <cstdint>
#ifndef RT3_H
# define RT3_H 1

# include <cassert>
# include <cmath>
# include <cstdlib>
# include <iostream>
# include <memory>
# include <vector>
using std::vector;
# include <array>
using std::array;
# include <map>
# include <stack>
# include <string>

# include <unordered_map>
using std::string;
# include <algorithm>
# include <tuple>
using std::tuple;
# include <utility>
#include "../ext/glm/glm.hpp"
#define GLM_FORCE_LEFT_HANDED
#include "image_io.h"

//== Alias to the chosen data structure to implement a dictionary.
// #define Dictionary std::unordered_map
# define Dictionary std::map

//=== Aliases
namespace rt3 {
/// ATENTION: This is just to compile the project. You need to implement
/// Point3f!!!!
// Alias to a Point3f (simulation)
using Point3f = glm::vec3;
Point3f operator - (Point3f a, float b);
Point3f operator + (Point3f a, float b);
// Temporary Vec3, just to compile. TODO: Implement a real one!!!
using Vector3f = glm::vec3;
using Color24 = std::array<int, 3>;
using Spectrum = std::array<float, 3>;
Spectrum operator * (Spectrum a, Spectrum b);
Spectrum operator + (Spectrum a, Spectrum b);
Spectrum operator * (Spectrum a, float b);
Spectrum operator * (float a, Spectrum b);
using Normal3f = glm::vec3;

// List of points
using ListPoint3f = std::vector<Point3f>;

// Temporary Vec3i. TODO: code a real one.
using Vector3i = glm::ivec3;
using Point3i = glm::ivec3;
using Point2i = glm::ivec2;
using Point2f = glm::vec2;

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& os, const std::array<T, S>& v) {
  os << "[ ";
  for (const auto& e : v) {
    os << e << " ";
  }
  os << "]";
  return os;
}

// Global Forward Declarations
class Film;
class Background;
class BackgroundColor;
class Camera;
class Primitive;
class Material;
class Surfel;
class Light;
class Scene;
class Bounds3f;
class Plane;
class Sphere;
class Shape;
class Scene;
class PrimList;
class GeometricPrimitive;
class AggregatePrimitive;

//=== aliases
using real_type = float;
using size_type = size_t;
using result_type = std::tuple<bool, std::string>;

/// This struct holds information provided via command line arguments
struct RunningOptions {
  // An alias template for a two-dimensional std::array
  template <typename T, std::size_t Row, std::size_t Col> using Array2d
    = std::array<std::array<T, Col>, Row>;

  // Vanilla ctro.
  RunningOptions() = default;

  // 1 = 100% of the full resolution.
  // [row=0] -> X; [row=1] -> Y
  // x0, x1, y0, y1
  Array2d<real_type, 2, 2> crop_window{ { { 0, 1 }, { 0, 1 } } };  //!< Crop window to render.
  std::string filename;                                            //!< input scene file name.
  std::string outfile;                                             //!< output image file name.
  bool quick_render{ false };  //!< if true render image with 1/4 of the
                               //!< requested resolition.
};

//=== Global Inline Functions

/*! Linear interpolation.
 * \param t The parameter, in [0,1].
 * \param v1 The initial interpolation value.
 * \param v2 The final interpolation value.
 * \return The interpolated value.
 */
//
inline float Lerp(float t, float v1, float v2) { return (1.F - t) * v1 + t * v2; }

/*! Linear interpolation between two Vectors.
 * \param t The parameter, in [0,1].
 * \param v1 The initial interpolation value.
 * \param v2 The final interpolation value.
 * \return The interpolated value.
 */
//
inline Vector3f Lerp(float t, Vector3f v1, Vector3f v2) { 
  return Vector3f(
    Lerp(t, v1[0], v2[0]),
    Lerp(t, v1[1], v2[1]),
    Lerp(t, v1[2], v2[2])
  );
}

/// Clamp T to [low,high].
template <typename T, typename U, typename V> inline T Clamp(T val, U low, V high) {
  if (val < low) {
    return low;
  }
  if (val > high) {
    return high;
  }
  return val;
}

/// Degrees to radians.
inline float Radians(float deg) { return ((float)M_PI / 180.F) * deg; }

/// Radians to degreees.
inline float Degrees(float rad) { return (180.F / (float)M_PI) * rad; }
}  // namespace rt3

#endif  // RT3_H
