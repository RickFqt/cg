#include "api.h"
#include "../backgrounds/background.h"
#include "parser.h"

#include <chrono>
#include <memory>

namespace rt3 {

//=== API's static members declaration and initialization.
API::APIState API::curr_state = APIState::Uninitialized;
RunningOptions API::curr_run_opt;
std::unique_ptr<RenderOptions> API::render_opt;
std::unique_ptr<Integrator> API::m_the_integrator;
std::unique_ptr<Scene> API::m_the_scene;
// GraphicsState API::curr_GS;

// THESE FUNCTIONS ARE NEEDED ONLY IN THIS SOURCE FILE (NO HEADER NECESSARY)
// ˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇˇ

Film* API::make_film(const ParamSet& ps) {
  std::cout << ">>> Inside API::make_film()\n";
  Film* film{ nullptr };
  std::vector<real_type> defult ={curr_run_opt.crop_window[0][0], curr_run_opt.crop_window[0][1], curr_run_opt.crop_window[1][0], curr_run_opt.crop_window[1][1]};
  film = create_film(ps, defult);

  // Return the newly created film.
  return film;
}

Material* API::make_material(const ParamSet& ps) {
  std::cout << ">>> Inside API::make_material()\n";
  Material* material{ nullptr };

  std::string type = retrieve(ps, "type", string{ "flat" });

  if(type == "flat"){
    material = create_flat_material(ps);
  }
  else if(type == "blinn"){
    material = create_blinn_phong_material(ps);
  }

  // std::cout << "???" << std::endl;

  // Return the newly created material.
  return material;
}

Shape* API::make_shape(const ParamSet &ps) {

  std::cout << ">>> Inside API::make_shape()\n";
  Shape* shape{ nullptr };

  std::string type = retrieve(ps, "type", string{ "sphere" });

  if(type == "sphere"){
    shape = create_sphere(ps);
  }
  else if(type == "triangle"){
    shape = create_simple_triangle(ps);
  }
  // TODO: Add new types here!

  return shape;
}

std::vector<std::shared_ptr<Shape>> API::make_shapes(const ParamSet &ps) {

  std::cout << ">>> Inside API::make_shapes()\n";
  std::vector<std::shared_ptr<Shape>> shapes;

  std::string type = retrieve(ps, "type", string{ "sphere" });

  if(type == "trianglemesh"){
    shapes = create_triangle_mesh_shape(false, ps); // TODO: Fix flip_normals
  }

  return shapes;
  
}

Primitive* API::make_object(const ParamSet &ps_obj, const ParamSet &ps_mat) {

  std::cout << ">>> Inside API::make_object()\n";

  std::shared_ptr<Shape> shape{make_shape(ps_obj)};
  std::shared_ptr<Material> material{make_material(ps_mat)};


  return new GeometricPrimitive(shape, material);
}

std::vector<std::shared_ptr<Primitive>> API::make_objects(const ParamSet &ps_obj, const ParamSet &ps_mat) {

  std::cout << ">>> Inside API::make_objects()\n";
  std::vector<std::shared_ptr<Primitive>> objects;

  std::vector<std::shared_ptr<Shape>> shapes{make_shapes(ps_obj)};
  std::shared_ptr<Material> material{make_material(ps_mat)};

  for(std::shared_ptr<Shape> shape : shapes){
    objects.push_back( std::shared_ptr<Primitive>(new GeometricPrimitive(shape, material)) );
  }

  return objects;
}

Primitive* API::make_aggregate(const std::vector<std::pair<ParamSet, ParamSet>>& vet_ps_obj_mat, const ParamSet& accel_ps){

  std::cout << ">>> Inside API::make_aggregate()\n";
  
  std::vector<std::shared_ptr<Primitive>> primitives;
  std::shared_ptr<Primitive> prim;
  std::vector<std::shared_ptr<Primitive>> prims;

  for(auto pair_ps : vet_ps_obj_mat){
    std::string type = retrieve(pair_ps.first, "type", string{ "sphere" });
    // Check if our object is a triangle_mesh. If so, several shapes will be created
    if(type == "trianglemesh"){
      prims = make_objects(pair_ps.first, pair_ps.second);
      for(std::shared_ptr<Primitive> p : prims){
        primitives.push_back(p);
      }
    }
    else{
      prim = std::shared_ptr<Primitive>( make_object(pair_ps.first, pair_ps.second) );
      primitives.push_back(prim);
    }
  }

  if(accel_ps.count("type") >= 1){

    std::cout << "Entrei aqui o\n";
    std::cout << primitives.size() << "\n";

    std::string type = retrieve(accel_ps, "type", string{ "bvh" });

    int max_prims = retrieve(accel_ps, "max_prims_per_node", int{ 4 } );

    return new BVHAccel(0, primitives.size(), primitives, max_prims);
  }

  return new PrimList(primitives);
}

Background* API::make_background(const ParamSet& ps) {
  std::cout << ">>> Inside API::make_background()\n";
  Background* bkg{ nullptr };
  bkg = create_color_background(ps);

  // Return the newly created background.
  return bkg;
}

Camera* API::make_camera(const ParamSet &cps, const ParamSet &lps, std::unique_ptr<Film>&& fml){
  std::cout << ">>> Inside API::make_camera()\n";
  Camera* camera{ nullptr };

  std::string type = retrieve(cps, "type", string{ "orthographic" });

  if(type == "orthographic"){
    camera = create_orthographic_camera(cps, lps, std::move(fml));
  }
  else{
    camera = create_perspective_camera(cps, lps, std::move(fml));
  }

  // Return the newly created camera.
  return camera;
}

Integrator* API::make_integrator(const ParamSet& ps, std::shared_ptr<const Camera> camera) {
  std::cout << ">>> Inside API::make_integrator()\n";
  Integrator* integrator{ nullptr };

  std::string type = retrieve(ps, "type", string{ "flat" });

  if(type == "flat"){
    integrator = create_flat_integrator(camera);
  }
  else if(type == "normal_map"){
    integrator = create_normal_map_integrator(camera);
  }
  else if(type == "depth_map"){
    integrator = create_depth_map_integrator(ps, camera);
  }
  else if(type == "blinn_phong"){
    integrator = create_blinn_phong_integrator(ps, camera);
  }

  return integrator;
}

Light* API::make_light(const ParamSet &ps){
  std::cout << ">>> Inside API::make_light()\n";
  Light* light{ nullptr };

  std::string type = retrieve(ps, "type", string{ "ambient" });

  if(type == "ambient"){
    light = create_ambient_light(ps);
  }
  else if(type == "point"){
    light = create_point_light(ps);
  }
  else if(type == "spot"){
    light = create_spot_light(ps);
  }
  else if(type == "directional"){
    light = create_directional_light(ps);
  }

  return light;
}

Scene* API::make_scene(std::shared_ptr< Background > bkg, std::shared_ptr<Primitive> agg, std::vector<ParamSet> l_ps) {
  std::cout << ">>> Inside API::make_scene()\n";
  
  std::vector< std::shared_ptr<Light>> v_light;
  for(ParamSet l : l_ps){
    v_light.push_back(std::shared_ptr<Light>(make_light(l)));
  }
  return new Scene(agg, bkg, v_light);
}
// ˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆˆ
// END OF THE AUXILIARY FUNCTIONS
// =========================================================================

//=== API's public methods implementation
void API::init_engine(const RunningOptions& opt) {
  // Save running option sent from the main().
  curr_run_opt = opt;
  // Check current machine state.
  if (curr_state != APIState::Uninitialized) {
    RT3_ERROR("API::init_engine() has already been called! ");
  }
  // Set proper machine state
  curr_state = APIState::SetupBlock;
  // Preprare render infrastructure for a new scene.
  render_opt = std::make_unique<RenderOptions>();
  // Create a new initial GS
  // curr_GS = GraphicsState();
  RT3_MESSAGE("[1] Rendering engine initiated.\n");
}

void API::clean_up() {
  // Check for correct machine state
  if (curr_state == APIState::Uninitialized) {
    RT3_ERROR("API::clean_up() called before engine initialization.");
  } else if (curr_state == APIState::WorldBlock) {
    RT3_ERROR("API::clean_up() called inside world definition section.");
  }
  curr_state = APIState::Uninitialized;

  RT3_MESSAGE("[4] Rendering engine clean up concluded. Shutting down...\n");
}

void API::run() {
  // Try to load and parse the scene from a file.
  RT3_MESSAGE("[2] Beginning scene file parsing...\n");
  // Recall that the file name comes from the running option struct.
  parse(curr_run_opt.filename.c_str());
}

void API::world_begin() {
  VERIFY_SETUP_BLOCK("API::world_begin");  // check for correct machine state.
  curr_state = APIState::WorldBlock;       // correct machine state.
}

void API::world_end() {
  VERIFY_WORLD_BLOCK("API::world_end");
  // The scene has been properly set up and the scene has
  // already been parsed. It's time to render the scene.

  // At this point, we have the background as a solitary pointer here.
  // In the future, the background will be parte of the scene object.
  std::shared_ptr<Background> the_background{ make_background(render_opt->bkg_ps) };

  std::shared_ptr<Primitive> aggregate{ make_aggregate(render_opt->list_objects_with_materials, render_opt->accelerator_ps)};

  // Same with the film, that later on will belong to a camera object.
  std::unique_ptr<Film> the_film = std::unique_ptr<Film>( make_film(render_opt->film_ps) );

  // Initialize camera
  std::shared_ptr<const Camera> the_camera = std::unique_ptr<Camera>( make_camera(render_opt->camera_ps, 
                                                    render_opt->look_at_ps, std::move(the_film)) );
  
  // Initialize integrator
  m_the_integrator = std::unique_ptr<Integrator>(make_integrator(render_opt->integrator_ps, the_camera));

  // Initialize scene
  m_the_scene = std::unique_ptr<Scene>(make_scene(the_background, aggregate, render_opt->list_lights_ps));
  
  // std::shared_ptr<BVHAccel> a = std::dynamic_pointer_cast< BVHAccel >( m_the_scene->get_aggregate() );
  // a->print_tree();
  // Run only if we got camera and background.
  if (m_the_integrator and m_the_scene) {
    RT3_MESSAGE("    Parsing scene successfuly done!\n");
    RT3_MESSAGE("[2] Starting ray tracing progress.\n");

    // Structure biding, c++17.
    auto res = the_camera->film->get_resolution();
    size_t w = res[0];
    size_t h = res[1];
    RT3_MESSAGE("    Image dimensions in pixels (W x H): " + std::to_string(w) + " x "
                + std::to_string(h) + ".\n");
    RT3_MESSAGE("    Ray tracing is usually a slow process, please be patient: \n");

    //================================================================================
    auto start = std::chrono::steady_clock::now();
    m_the_integrator->render( *m_the_scene );
    auto end = std::chrono::steady_clock::now();
    //================================================================================
    auto diff = end - start;  // Store the time difference between start and end
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    RT3_MESSAGE("    Time elapsed: " + std::to_string(diff_sec.count()) + " seconds ("
                + std::to_string(std::chrono::duration<double, std::milli>(diff).count())
                + " ms) \n");
  }
  // [4] Basic clean up
  curr_state = APIState::SetupBlock;  // correct machine state.
  reset_engine();
}

/// This api function is called when we need to re-render the *same* scene (i.e.
/// objects, lights, materials, etc) , maybe with different integrator, and
/// camera setup. Hard reset on the engine. User needs to setup all entities,
/// such as camera, integrator, accelerator, etc.
void API::reset_engine() {
  // curr_GS = GraphicsState();
  // This will delete all information on integrator, cameras, filters,
  // acceleration structures, etc., that has been set previously.
  render_opt = std::make_unique<RenderOptions>();
}

void API::background(const ParamSet& ps) {
  std::cout << ">>> Inside API::background()\n";
  VERIFY_WORLD_BLOCK("API::background");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->bkg_type = type;
  // Store current background object.
  render_opt->bkg_ps = ps;
}

void API::object(const ParamSet& ps) {
  std::cout << ">>> Inside API::object()\n";
  VERIFY_WORLD_BLOCK("API::object");

  // Store current object into the list of objects.
  render_opt->list_objects_with_materials.push_back({ps, render_opt->curr_material});
}

void API::camera(const ParamSet& ps) {
  std::cout << ">>> Inside API::camera()\n";
  VERIFY_SETUP_BLOCK("API::camera");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->camera_type = type;
  // Store current camera object.
  render_opt->camera_ps = ps;
}

void API::look_at(const ParamSet& ps) {
  std::cout << ">>> Inside API::look_at()\n";
  VERIFY_SETUP_BLOCK("API::look_at");

  // Store current look_at object.
  render_opt->look_at_ps = ps;
}

void API::film(const ParamSet& ps) {
  std::cout << ">>> Inside API::film()\n";
  VERIFY_SETUP_BLOCK("API::film");

  // retrieve type from ps.
  std::string type = retrieve(ps, "type", string{ "unknown" });
  render_opt->film_type = type;
  render_opt->film_ps = ps;
}

void API::light_source(const ParamSet &ps) {
  std::cout << ">>> Inside API::light_source()\n";
  VERIFY_WORLD_BLOCK("API::light_source");

  render_opt->list_lights_ps.push_back(ps);
}

void API::make_named_material(const ParamSet &ps){
  std::cout << ">>> Inside API::make_named_material()\n";
  VERIFY_WORLD_BLOCK("API::make_named_material");

  std::string name = retrieve(ps, "name", string{ "unknown" });

  // Add the new named material into the library
  render_opt->material_library[name] = ps;
}

void API::named_material(const ParamSet &ps){
  std::cout << ">>> Inside API::named_material()\n";
  VERIFY_WORLD_BLOCK("API::named_material");

  std::string name = retrieve(ps, "name", string{ "unknown" });

  // If there is no already created material, we create one
  if(render_opt->material_library.count(name) < 1){
    std::cout << "Named material not found! Using default material...\n";
    // TODO: Check if this works
    render_opt->curr_material = ParamSet();
  }
  else{
    // Set the current material to the one specified (named)
    render_opt->curr_material = render_opt->material_library[name];
  }

}

void API::material(const ParamSet &ps){
  std::cout << ">>> Inside API::material()\n";
  VERIFY_WORLD_BLOCK("API::material");

  // Set the current material to the one specified (anonymous)
  render_opt->curr_material = ps;
}

void API::integrator(const ParamSet &ps) {
  std::cout << ">>> Inside API::integrator()\n";
  VERIFY_SETUP_BLOCK("API::integrator");
  render_opt->integrator_ps = ps;
}

void API::accelerator(const ParamSet &ps) {
  std::cout << ">>> Inside API::integrator()\n";
  VERIFY_SETUP_BLOCK("API::accelerator");
  render_opt->accelerator_ps = ps;
}

}  // namespace rt3
