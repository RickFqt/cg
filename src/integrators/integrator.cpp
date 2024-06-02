#include "integrator.h"
#define MAX_CARACTERES 50

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

    // Used for load bar
    int final_pixel = (h_final - h_init) * (w_final - w_init);
    int current_pixel = 0;
    int progress = 0;
    int last_progress = -1;
    // std::cout << "AAAAAAAAAAAAA " << h_init <<  " " <<  h_final << " " << w_init << " " << w_final << "\n";
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
            current_pixel++;
            
            // The loading bar
            progress = (MAX_CARACTERES * current_pixel) / final_pixel;
            if(progress != last_progress){
                last_progress = progress;
                for(int i=0; i < progress; ++i){
                    std::cout << "#";
                }
                for(int i=progress; i < MAX_CARACTERES; ++i){
                    std::cout << ".";
                }
                std::cout << "\n\n";
            }

        }
    }
    // Send image color buffer to the output file.
    camera->film->write_image();
}

// This method must be overridden by all classes derived from SamplerIntegrator.
/// Determines a color for the incoming ray.
std::optional<Color24> FlatIntegrator::Li(const Ray& ray, const Scene& scene) const
{
    // zmin <!> 4 <!> zmax não é verdade
    // (4-0.1)/(0.8-0.1)
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

// This method must be overridden by all classes derived from SamplerIntegrator.
/// Determines a color for the incoming ray.
std::optional<Color24> NormalMapIntegrator::Li(const Ray& ray, const Scene& scene) const
{
    Color24 L{0,0,0}; // The radiance
    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.
    if (!scene.intersect(ray, &isect)) {
        return {}; // empty object.
    }
    
    // Get the surface normal (normally normalized)
    Point3f normal = glm::normalize(isect.n);

    // Treat the coordinates as RGB color, where values in [-1,1]↦[0,255].
    L[0] = (normal[0] + 1) * ((float)255/(float)2);
    L[1] = (normal[1] + 1) * ((float)255/(float)2);
    L[2] = (normal[2] + 1) * ((float)255/(float)2);
    
    return L;
}

std::optional<Color24> DepthMapIntegrator::Li(const Ray& ray, const Scene& scene) const
{
    Color24 L{0,0,0}; // The radiance
    // Find closest ray intersection or return background radiance.
    Surfel isect; // Intersection information.
    if (!scene.intersect(ray, &isect)) {
        return {}; // empty far color.
    }
    auto z = ray.get_t_max();
    z = (z-z_buffer_min)/(z_buffer_max-z_buffer_min);
    if(z <= z_min){
        L =near_color;
    }else if(z >= z_max){
        L  = far_color;
    }else{
        uint8_t r = (uint8_t)Lerp(z, near_color[0], far_color[0]);
        uint8_t g = (uint8_t)Lerp(z, near_color[1], far_color[1]);
        uint8_t b = (uint8_t)Lerp(z, near_color[2], far_color[2]);
        L = {r, g, b};
    }
    return L;
}

void DepthMapIntegrator::preprocess(const Scene& scene){
    Point2i img_dim = camera->film->get_resolution(); // Retrieve the image dimensions in pixels.
    real_type x0 = camera->film->m_vcrop[0];
    real_type x1 = camera->film->m_vcrop[1];
    real_type y0 = camera->film->m_vcrop[2];
    real_type y1 = camera->film->m_vcrop[3];
    size_t w_init = round(img_dim[0]*(x0));
    size_t h_init = round(img_dim[1]*(y0));
    size_t w_final = round(img_dim[0]*(x1));
    size_t h_final = round(img_dim[1]*(y1));
    z_buffer_min = INFINITY;
    z_buffer_max = 0;

    z_buffer = std::vector<std::vector<real_type>>(h_final-h_init,std::vector<real_type>(w_final-w_init, INFINITY));
    // This might just be a tile (part) of an image, rendered in parallel.
    for ( size_t y = h_init ; y < h_final ; y++ ) {
        for( size_t x = w_init ; x < w_final ; x++ ) {
            // Determine the ray for the current camera type.
            Ray ray = camera->generate_ray( x, y ); // Generate the ray from (x,y)
            Surfel isect; // Intersection information.
            if (scene.intersect(ray, &isect)) {
                z_buffer[y-h_init][x-w_init] = ray.get_t_max();
                if(z_buffer[y-h_init][x-w_init] < z_buffer_min) z_buffer_min = z_buffer[y-h_init][x-w_init];
                if(z_buffer[y-h_init][x-w_init] > z_buffer_max) z_buffer_max = z_buffer[y-h_init][x-w_init];
            }
        }
    }
}

FlatIntegrator* create_flat_integrator(std::shared_ptr<const Camera> cam){

    return new rt3::FlatIntegrator(cam);
}

NormalMapIntegrator* create_normal_map_integrator(std::shared_ptr<const Camera> cam){

    return new rt3::NormalMapIntegrator(cam);
}

DepthMapIntegrator* create_depth_map_integrator(const ParamSet& ps, std::shared_ptr<const Camera> cam){

    real_type z_min = retrieve(ps, "z_min", 0.F);
    real_type z_max = retrieve(ps, "z_max", 1.F);

    Color24 near_color = retrieve(ps, "near_color", Color24{0,0,0});
    Color24 far_color = retrieve(ps, "far_color", Color24{255,255,255});

    return new rt3::DepthMapIntegrator(cam, z_min, z_max, near_color, far_color);
}

}   // namespace rt3