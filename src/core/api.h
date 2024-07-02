#ifndef API_H
#define API_H 1

#include <string>
#include <map>

#include "paramset.h"
#include "rt3.h"
#include "../integrators/integrators.h"
#include "../shapes/sphere.h"
#include "../shapes/simple_triangle.h"
#include "../shapes/triangle.h"
#include "../primitives/BVHAccel.h"
#include "../primitives/primlist.h"
#include "transform.h"

//=== API Macro definitions

/// Check whether the current state has been intialized.
#define VERIFY_INITIALIZED(func_name)                                          \
  if (curr_state == APIState::Uninitialized) {                                 \
    RT3_ERROR(std::string{"API::init() must be called before "} +              \
              std::string{func_name} + std::string{"(). Ignoring."});          \
  }

/// Check whether the current state corresponds to setup section.
#define VERIFY_SETUP_BLOCK(func_name)                                          \
  VERIFY_INITIALIZED(func_name)                                                \
  if (curr_state == APIState::WorldBlock) {                                    \
    RT3_ERROR(std::string{"Rendering setup cannot happen inside "} +           \
              std::string{"World definition block; "} +                        \
              std::string{func_name} +                                         \
              std::string{"() not allowed. Ignoring"});                        \
  }

/// Check whether the current state corresponds to the world section.
#define VERIFY_WORLD_BLOCK(func_name)                                          \
  VERIFY_INITIALIZED(func_name)                                                \
  if (curr_state == APIState::SetupBlock) {                                    \
    RT3_ERROR(std::string{"Scene description must happen inside "} +           \
              std::string{"World Definition block; "} +                        \
              std::string{func_name} +                                         \
              std::string{"() not allowed. Ignoring"});                        \
  }

namespace rt3 {
/// Collection of objects and diretives that control rendering, such as camera,
/// lights, prims.
struct RenderOptions {
  // the Film
  std::string film_type{"image"}; // The only type available.
  ParamSet film_ps;
  /// the Camera
  string camera_type{"perspective"};
  ParamSet camera_ps;
  /// the look_at
  ParamSet look_at_ps;
  /// the Bakcground
  string bkg_type{"solid"}; // "image", "interpolated"
  ParamSet bkg_ps;
  /// the Objects with its associated materials
  std::vector<std::pair<ParamSet, ParamSet>> list_objects_with_materials;
  /// the Integrator
  ParamSet integrator_ps;
  /// the Accelerator Structure
  ParamSet accelerator_ps;
  /// the library of Materials
  std::map<string, ParamSet> material_library;
  /// the current Material
  ParamSet curr_material;
  /// the list of Lights
  std::vector<ParamSet> list_lights_ps;
};

/// Collection of data related to a Graphics state, such as current material,
/// lib of material, etc.
struct GraphicsState {
  std::shared_ptr< Material > curr_material;  //!< Current material that globally affects all objects.
	bool flip_normals{false};              //!< When true, we flip the normals
	using DictOfMat = Dictionary< string, std::shared_ptr<Material> >;
	std::shared_ptr< DictOfMat > mats_lib;      //!< Library of materials.
	bool mats_lib_cloned{false};           //!< We only actually clone the library if a new material is added to it.
};

/// Static class that manages the render process
class API {
public:
  /// Defines the current state the API may be at a given time
  enum class APIState {
    Uninitialized, //!< Initial state, before parsing.
    SetupBlock,    //!< Parsing the render setup section.
    WorldBlock     //!< Parsing the world's information section.
  };

  /// Stores the running options collect in main().
  static RunningOptions curr_run_opt;

private:
  /// Current API state
  static APIState curr_state;
  /*
   * The unique pointer below is useful to support various calls to
   * init()-run()-clean-up(), in case we want to process several input files in
   * a single run of the system.
   */
  /// Unique infrastructure to render a scene (camera, integrator, etc.).
  static std::unique_ptr<RenderOptions> render_opt;
  static std::unique_ptr<Integrator> m_the_integrator;
  /// Pointer to the scene. We keep it as parte of the API because it may be
  // reused later [1] Create the integrator.
  static std::unique_ptr< Scene > m_the_scene;

  /// The Current Transformation Matrix
	static Transform curr_TM;
	/// Named coordinate systems dictionary
	static Dictionary< string, Transform > named_coord_system;
	/// The current GraphicsState
	static GraphicsState curr_GS;
	/// The stack of GraphicsState, activate by the tags `<pushGS/>...<popGS/>`
	static std::stack< GraphicsState > saved_GS; // Recall that the GS includes the curent transformation.
	/// The stack of transformations, activate by the tags `<pushCTM/>...<popCTM/>`
	static std::stack< Transform > saved_TM;

  /* --------------------------------------------------------------------------------
	* The matrix lookup is unique map (hash table) of transformation matrices.
	* Every new transformation that is created in `API::transform()`
	* should be stored in this map.
	* So, whenever we generate a transformation matrix (either defined
	* directly in the scene file or as a result of composition of other
	* matrices), we do the following: we look it up in the dictionary;
	* if it's there, we return the shared pointer stored in the map;
	* if it's NOT there yet, we store it in the dictionary and return
	* the shared pointer stored in the map.
	* -------------------------------------------------------------------------------- */
	/// The Dictionary of instantiated transformation matrix.
	static Dictionary< string, std::shared_ptr< const Transform > > transformation_cache;


  // === Helper functions.
  ///
  static Film *make_film(const ParamSet &ps);
  static Background *make_background(const ParamSet &ps);
  static Camera *make_camera(const ParamSet &cps, const ParamSet &lps, std::unique_ptr<Film>&& fml);
  static Primitive *make_object(const ParamSet &ps_obj, const ParamSet &ps_mat);
  static std::vector<std::shared_ptr<Primitive>> make_objects(const ParamSet &ps_obj, const ParamSet &ps_mat);
  static Shape *make_shape(const ParamSet &ps);
  static std::vector<std::shared_ptr<Shape>> make_shapes(const ParamSet &ps);
  static Material *make_material(const ParamSet &ps);
  static Light *make_light(const ParamSet &ps);
  static Primitive *make_aggregate(const std::vector<std::pair<ParamSet, ParamSet>>& vet_ps_obj_mat, const ParamSet &accel_ps);
  static Integrator *make_integrator(const ParamSet &ps, std::shared_ptr<const Camera> camera);
  static Scene *make_scene(std::shared_ptr< Background > bkg, std::shared_ptr<Primitive> agg, std::vector<ParamSet> l_ps);

public:
  //=== API function begins here.
  static void init_engine(const RunningOptions &);
  static void run();
  static void clean_up();
  static void reset_engine();
  
  //=== CTM functions.
  static void identity();
  static void translate(const ParamSet &ps);
  static void scale(const ParamSet &ps);
  static void rotate(const ParamSet &ps);
  static void save_coord_system(const ParamSet &ps);
  static void restore_coord_system(const ParamSet &ps);

  // === CTM & GS Stack functions
  static void push_CTM();
  static void pop_CTM();
  static void push_GS();
  static void pop_GS();

  static void film(const ParamSet &ps);
  static void camera(const ParamSet &ps);
  static void look_at(const ParamSet &ps);
  static void background(const ParamSet &ps);
  static void object(const ParamSet &ps);
  static void make_named_material(const ParamSet &ps);
  static void named_material(const ParamSet &ps);
  static void material(const ParamSet &ps);
  static void light_source(const ParamSet &ps);
  static void integrator(const ParamSet &ps);
  static void accelerator(const ParamSet &ps);
  static void world_begin();
  static void world_end();
};
} // namespace rt3

#endif // API_H
