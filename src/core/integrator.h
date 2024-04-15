#ifndef INTEGRATOR_H
#define INTEGRATOR_H 1

#include "primitive.h"
#include "ray.h"
#include "rt3.h"
#include "scene.h"
#include "camera.h"
#include "background.h"
#include "material.h"
#include <optional>

namespace rt3{


class Integrator {
	public:
		Integrator(){}
		virtual ~Integrator();
		virtual void render( const Scene& scene ) =0;
};

class SamplerIntegrator : public Integrator {
	//=== Public interface
	public:
		virtual ~SamplerIntegrator();
		SamplerIntegrator( std::shared_ptr<const Camera> cam ) : camera{cam}{/* empty */};

		virtual std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const = 0;
		virtual void render( const Scene& scene );
		virtual void preprocess( const Scene& scene );

	protected:
		std::shared_ptr<const Camera> camera;
};

class FlatIntegrator : public SamplerIntegrator {

	//=== Public interface
	public:
		virtual ~FlatIntegrator();
		FlatIntegrator( std::shared_ptr<const Camera> cam): SamplerIntegrator(cam){/* empty */}

		std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
};
// factory pattern functions.
FlatIntegrator* create_flat_integrator(std::shared_ptr<const Camera> cam);

} // namespace rt3

#endif