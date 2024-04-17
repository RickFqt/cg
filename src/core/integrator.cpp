#include "integrator.h"

namespace rt3 {

// Enter the ray tracing main loop; this is the default render() method.
// Notice that the FlatIntegrator does not need to override this method.
// It relies on the version provided by the base class (SamplerIntegrator).
void SamplerIntegrator::render(const Scene& scene) {
    // Always call the preprocess() before doing any rendering.
    // This might be just an empty method, or not, depending on the integrator's needs.
    preprocess(scene);

    Point2i img_dim = camera->film->get_resolution(); // Retrieve the image dimensions in pixels.
    real_type x0 = camera->film->m_vcrop[0];
    real_type x1 = camera->film->m_vcrop[1];
    real_type y0 = camera->film->m_vcrop[2];
    real_type y1 = camera->film->m_vcrop[3];
    size_t w_init = round(img_dim[0]*(x0));
    size_t h_init = round(img_dim[1]*(y0));
    size_t w_final = round(img_dim[0]*(x1));
    size_t h_final = round(img_dim[1]*(y1));

    // This might just be a tile (part) of an image, rendered in parallel.
    for ( size_t y = h_init ; y < h_final ; y++ ) {
        for( size_t x = w_init ; x < w_final ; x++ ) {
            // Determine the ray for the current camera type.
            Point2f screen_coord{ float(x)/float(img_dim.x), float(y)/float(img_dim.y) };
            Ray ray = camera->generate_ray( x, y ); // Generate the ray from (x,y)
            // Determine the incoming light.
            auto temp_L =  Li( ray, scene );
            Color24 L = (temp_L.has_value()) ?  temp_L.value() : scene.background->sampleXYZ(screen_coord) ;
            // Add color (radiance) to the image.
            camera->film->add_sample( Point2i( x, y ), L ); // Set color of pixel (x,y) to L.
        }
    }
    // Send image color buffer to the output file.
    camera->film->write_image();
}

// This method must be overridden by all classes derived from SamplerIntegrator.
/// Determines a color for the incoming ray.
std::optional<Color24> FlatIntegrator::Li(const Ray& ray, const Scene& scene) const
{
    Color24 L{0,0,0}; // The radiance
    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.
    if (!scene.intersect(ray, &isect)) {
        return {}; // empty object.
    }
    // Some form of determining the incoming radiance at the ray's origin.
    // Polymorphism in action.
    FlatMaterial * fm = dynamic_cast< FlatMaterial *>( isect.primitive->get_material() );
    // Assign diffuse color to L.
    L = fm->get_kd(); // Call a method present only in FlatMaterial.
    return L;
}

FlatIntegrator* create_flat_integrator(std::shared_ptr<const Camera> cam){

    return new rt3::FlatIntegrator(cam);
}

}   // namespace rt3