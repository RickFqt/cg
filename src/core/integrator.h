#ifndef INTEGRATOR_H
#define INTEGRATOR_H 1

#include "primitive.h"
#include "ray.h"
#include "rt3.h"
#include "scene.h"
#include "../cameras/camera.h"
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
	real_type z_min, z_max;
	real_type z_buffer_min, z_buffer_max;
	Color24 near_color, far_color;
	std::vector<std::vector<real_type>> z_buffer;

//=== Public interface
public:
	virtual ~DepthMapIntegrator(){};
	DepthMapIntegrator( std::shared_ptr<const Camera> cam, const real_type& zmin, const real_type& zmax, const Color24& nearcolor, 
						const Color24& maxcolor):
		SamplerIntegrator(cam), z_min{zmin}, z_max{zmax}, near_color{nearcolor}, far_color{maxcolor}{/* empty */}

	std::optional<Color24> Li( const Ray& ray, const Scene& scene ) const;
	
	void preprocess( const Scene& scene );
};
// factory pattern functions.
FlatIntegrator* create_flat_integrator(std::shared_ptr<const Camera> cam);

NormalMapIntegrator* create_normal_map_integrator(std::shared_ptr<const Camera> cam);

DepthMapIntegrator* create_depth_map_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam);

} // namespace rt3

#endif