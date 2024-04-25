#ifndef SURFEL_H
#define SURFEL_H 1

#include "rt3.h"
#include "ray.h"
#include "primitive.h"



namespace rt3{

using rt3::Point3f;
using rt3::Vector3f;

/* 
 Represents the geometry of a particular point on a surface, often a position found by 
 intersecting a ray against the surface. 
 This abstraction allows us to pass the contact information along to other parts of the renderer 
 without necessary having to know which object has been hit.
*/
class Surfel{
	public:
		Surfel( const Point3f&p,const Vector3f&n, const Vector3f&wo, float time,
			const Point2f& uv, const Primitive *pri )
			: p{p}, n{n}, wo{wo}, time{time}, uv{uv}, primitive{pri}
            {/* empty */};
		Surfel(){/* empty */}

		Point3f p;        //!< Contact point.
		Vector3f n;       //!< The surface normal (normally normalized).
		Vector3f wo;      //!< Outgoing direction of light, which is -ray.
        float time;
		Point2f uv;     //!< Parametric coordinate (u,v) of the hit surface.
		const Primitive *primitive=nullptr; //!< Pointer to the primitive.
};

//-------------------------------------------------------------------------------
} // namespace rt3

#endif