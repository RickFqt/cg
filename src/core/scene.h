#ifndef SCENE_H
#define SCENE_H 1

#include "ray.h"
#include "../primitives/primitive.h"

namespace rt3{

class Scene {
    //=== Public data
    public:
        //std::vector<shared_ptr<Light>> lights; // list of lights
        std::shared_ptr< Background > background; // The background object.
    private:
        std::shared_ptr<Primitive> aggregate; // The scene graph of objects, acceleration structure.

    //=== Public interface
    public:
        Scene( std::shared_ptr<Primitive> ag, std::shared_ptr< Background > bkg)
             : background{bkg}, aggregate{ag}
        {/* empty */}
        /// Determines the intersection info; return true if there is an intersection.
        bool intersect( const Ray& r, Surfel *isect ) const
        {return aggregate->intersect(r, isect);};
        /*! A faster version that only determines whether there is an intersection or not;
         * it doesn't calculate the intersection info.
         */
        bool intersect_p( const Ray& r ) const
        {return aggregate->intersect_p(r);};

        std::shared_ptr<Primitive> get_aggregate() const {return aggregate;}
};

} // namespace rt3

#endif