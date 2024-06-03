/// @file triangle.

#ifndef TRIANGLE_H
#define TRIANGLE_H

// #include "../ext/tiny_obj_loader.h"
#include "../core/rt3.h"
#include "../primitives/primitive.h"
#include "../core/paramset.h"
#include "../ext/tiny_obj_loader.h"

namespace rt3{

/// This struct implements an indexd triangle mesh database.
struct TriangleMesh {
  /// # of triangles in the mesh.
  int n_triangles;
  // The size of the three lists below should be 3 * nTriangles. Every 3 values
  // we have a triangle.
  vector<int> vertex_indices;  //!< The list of indices to the vertex list, for
                               //!< each individual triangle.
  vector<int> normal_indices;  //!< The list of indices to the normals list, for
                               //!< each individual triangle.
  vector<int> uvcoord_indices; //!< The list of indices to the UV coord list,
                               //!< for each individual triangle.

  // Here comes the data associated with each vertex. WARNING: The length of the
  // vectors below may differ.
  vector<Point3f> vertices; //!< The 3D geometric coordinates
  vector<Normal3f> normals; //!< The 3D normals.
  vector<Point2f> uvcoords; //!< The 2D texture coordinates.

  // Regular constructor (almost all data is imported via move semantics).
  TriangleMesh() = default;
  /// Copy constructor disabled
  TriangleMesh(const TriangleMesh &) = delete;
  /// Assign operator disabled.
  TriangleMesh &operator=(const TriangleMesh &) = delete;
  /// Move constructor.
  TriangleMesh(TriangleMesh &&other) = delete;
};

/// Represents a single triangle.
class Triangle : public Shape {
private:
  /// Pointer to the vertex index array stored in the triangle mesh database.
  int *v;
  // After the proper triangle initialization, we may access vertices with v[0],
  // v[1], and v[2]

  /// Pointer to the normal index array stored in the triangle mesh database.
  int *n;
  // After the proper triangle initialization, we may access normals with n[0],
  // n[1], and n[2]

  /// Pointer to the UV coord index array stored in the triangle mesh database.
  int *uv;
  // After the proper triangle initialization, we may access UV coords with
  // uv[0], uv[1], and uv[2]

  /// This is the **indexed triangle mesh database** this triangle is linked to.
  std::shared_ptr<TriangleMesh> mesh;
  /// Set it `true` to activate backface culling.
  bool backface_cull;

public:
  // The single constructor, that receives the mesh, this triangle id, and an
  // indication for backface culling.
  Triangle(std::shared_ptr<TriangleMesh> mesh, int tri_id, bool bfc = true, bool fn = false)
      : Shape(fn), mesh{mesh}, backface_cull{bfc} {
    // This is just a shortcut to access this triangle's data stored in the mesh
    // database.
    v = &mesh->vertex_indices[3 * tri_id];
    n = &mesh->normal_indices[3 * tri_id];
    uv = &mesh->uvcoord_indices[3 * tri_id];
  }
  /// Return the triangle's bounding box.
  Bounds3f world_bounds();
  /// The regular intersection methods, as defined in the Shape parent class.
  bool intersect(const Ray &r, float *t_hit, Surfel *sf) const;
  [[nodiscard]] bool intersect_p(const Ray &r) const override;

  /// This friend function helps us debug the triangles, if we want to.
  friend std::ostream &operator<<(std::ostream &os, const Triangle &t);
};

/// This is the entry point for the client. This function begins the process of
/// reading a triangle mesh.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals,
                                                     const ParamSet &ps);

/// This is the function that actually creates the mesh database and the
/// triangles, ans store them in a Shape list.
std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh, bool bfc, bool fn);

/// Internal function that calls the tinyobjloader api to read the OBJ data into
/// memory.
bool load_mesh_data(const std::string &filename, bool rvo, bool cn, bool fn,
                    std::shared_ptr<TriangleMesh> md);

/// This function essentially converts the mesh data from the tinyobjloader
/// internal data structure to our mesh data structure.
void extract_obj_data(const tinyobj::attrib_t &attrib,
                      const std::vector<tinyobj::shape_t> &shapes, bool rvo,
                      bool cn, bool fn, std::shared_ptr<TriangleMesh> md);

} // namespace rt3

#endif
