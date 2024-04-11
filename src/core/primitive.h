#ifndef PRIMITIVE_H
#define PRIMITIVE_H 1

#include "./rt3.h"
#include "ray.h"
#include "surfel.h"
#include "material.h"
#include "shape.h"

namespace rt3{

/*
    Represents any surface that a ray might hit.
    Any class derived from Primitive should provide a method to determine the intersection of 
    a ray with the surface, as well as return a pointer to the material object associated with the surface
*/
class Primitive {
	public:
        Primitive(){ /* empty */ }
		virtual ~Primitive(){ /* empty */ };
		virtual bool intersect( const Ray& r, Surfel *sf ) const = 0;
		// Simpler & faster version of intersection that only return true/false.
		// It does not compute the hit point information.
		virtual bool intersect_p( const Ray& r ) const = 0;
		virtual std::shared_ptr<Material> get_material() const = 0;
};

class GeometricPrimitive : public Primitive {
private:
	std::shared_ptr<Shape> shape;
	std::shared_ptr<Material> material;

public:
	GeometricPrimitive(std::shared_ptr<Shape> s, std::shared_ptr<Material> m)
	:shape{s}, material{m}
	{ /* empty */ };

	// Bounds3f world_bounds(); TODO

	bool intersect_p( const Ray& r ) const;
    bool intersect( const Ray& r, Surfel *sf ) const;
	std::shared_ptr<Material> get_material() const {return material;};
	void set_material(std::shared_ptr<Material> m) {material = m;};

};

//-------------------------------------------------------------------------------
} // namespace rt3

#endif