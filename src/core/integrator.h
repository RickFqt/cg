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
	Integrator() = default;
	virtual ~Integrator(){};
	virtual void render( const Scene& scene ) =0;
};

class SamplerIntegrator : public Integrator {
//=== Public interface
public:
	virtual ~SamplerIntegrator(){};
	SamplerIntegrator( std::shared_ptr<const Camera> cam ) : camera{cam}{};

	virtual std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const = 0;
	virtual void render( const Scene& scene );
	virtual void preprocess( const Scene& scene ){};

protected:
	std::shared_ptr<const Camera> camera;
};

class FlatIntegrator : public SamplerIntegrator {

//=== Public interface
public:
	virtual ~FlatIntegrator(){};
	FlatIntegrator( std::shared_ptr<const Camera> cam): SamplerIntegrator(cam){/* empty */}

	std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
};

class NormalMapIntegrator : public SamplerIntegrator {

//=== Public interface
public:
	virtual ~NormalMapIntegrator(){};
	NormalMapIntegrator( std::shared_ptr<const Camera> cam): SamplerIntegrator(cam){/* empty */}

	std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
};

class DepthMapIntegrator : public SamplerIntegrator {
private:
	// TODO: Put z_min, z_max, near_color & max_color here

//=== Public interface
public:
	virtual ~DepthMapIntegrator(){};
	// TODO: Change ctro so it contains z_min, zmax etc
	DepthMapIntegrator( std::shared_ptr<const Camera> cam):SamplerIntegrator(cam){/* empty */}

	std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
	// TODO: Make preprocess function and Li implementation
};
// factory pattern functions.
FlatIntegrator* create_flat_integrator(std::shared_ptr<const Camera> cam);

NormalMapIntegrator* create_normal_map_integrator(std::shared_ptr<const Camera> cam);

DepthMapIntegrator* create_depth_map_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam);

} // namespace rt3

#endif