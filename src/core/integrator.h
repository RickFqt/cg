#ifndef INTEGRATOR_H
#define INTEGRATOR_H 1

#include "ray.h"
#include "surfel.h"
#include "rt3.h"
#include "scene.h"
#include "camera.h"

namespace rt3{


class Integrator {
	public:
		virtual ~Integrator();
		virtual void render( const Scene& scene ) =0;
};

class SamplerIntegrator : public Integrator {
	//=== Public interface
	public:
		virtual ~SamplerIntegrator();
		SamplerIntegrator( std::shared_ptr<const Camera> cam ) : camera{cam}{/* empty */};

		virtual Color24 Li( const Ray& ray, const Scene& scene ) const = 0;
		virtual void render( const Scene& scene );
		virtual void preprocess( const Scene& scene );

	protected:
		std::shared_ptr<const Camera> camera;
};

} // namespace rt3

#endif