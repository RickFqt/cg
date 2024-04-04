#ifndef PRIMITIVE_H
#define PRIMITIVE_H 1

#include "./rt3.h"
#include "ray.h"
#include "surfel.h"

namespace rt3{

/*
    Represents any surface that a ray might hit.
    Any class derived from Primitive should provide a method to determine the intersection of 
    a ray with the surface, as well as return a pointer to the material object associated with the surface
*/
class Primitive {
	public:
		virtual ~Primitive();
		virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;
		// TODO: virtual const Material * get_material(void) const = { return material; }
	private:
		// TODO: std::shared_ptr<Material> material;
};

//-------------------------------------------------------------------------------
} // namespace rt3

#endif