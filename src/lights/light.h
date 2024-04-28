#ifndef LIGHT_H
#define LIGHT_H 1

#include "../core/surfel.h"
#include "../core/scene.h"
#include "../core/ray.h"
#include "../core/rt3.h"

namespace rt3 {

enum class light_flag_e : int {
	point = 1,
	directional = 2,
	area = 4,
	ambient = 8,
	spot = 16
};

// bool is_ambient( int flag ) {
// 	return flag & (int) light_flag_e::ambient;
// }

// Verifica se há oclusão entre dois pontos de contato.
class VisibilityTester {
public:
  VisibilityTester()=default;
  VisibilityTester( const Surfel& s0, const Surfel& s1):p0{s0}, p1{s1}{/* empty */};
  bool unoccluded( const Scene& scene );
public:
  Surfel p0, p1;
};

class Light {
public:
  light_flag_e flags;
public:
  virtual ~Light(){};
  Light( int flags ): flags{flags} {/* empty */};
  /// Retorna a intensidade da luz, direção e o teste oclusão.
  virtual Color24 sample_Li( const Surfel&   hit  /*in*/,
                             Vector3f        *wi  /*out*/,
                             VisibilityTester *vis /*out*/ ) = 0;
  virtual void preprocess( const Scene & ) {};
};

} // namespace rt3

#endif