#include <chrono>
#include <iostream>
#include <iterator>

#include "triangle.h"

namespace rt3 {

bool Triangle::intersect(const Ray &r, float *t_hit, Surfel *sf) const{

    // This is how we retrieve the information associated with this particular triangle.
    const Point3f &p0 = mesh->vertices[v[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
    const Point3f &p1 = mesh->vertices[v[1]]; // Same for the 1-vertex.
    const Point3f &p2 = mesh->vertices[v[2]]; // Same for the 2-vertex.
    // Let us retrieve the normals in the same way.
    const Normal3f &n0 = mesh->normals[n[0]]; // Retrieve the normal at vertex 0.
    const Normal3f &n1 = mesh->normals[n[1]]; // Retrieve the normal at vertex 1.
    const Normal3f &n2 = mesh->normals[n[2]]; // Retrieve the normal at vertex 2.

    //TODO: This isn't used yet
    // Same goes for the UV coords.
    const Point2f &uv0 = mesh->uvcoords[uv[0]]; // Retrieve the uv coord at vertex 0.
    const Point2f &uv1 = mesh->uvcoords[uv[1]]; // Retrieve the uv coord at vertex 1.
    const Point2f &uv2 = mesh->uvcoords[uv[2]]; // Retrieve the uv coord at vertex 2.

    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;
    Vector3f ray_cross_e2 = glm::cross(r.get_direction(), edge2);
    float det = glm::dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    Vector3f s = r.get_origin() - p0;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    Vector3f s_cross_e1 = glm::cross(s, edge1);
    float v = inv_det * glm::dot(r.get_direction(), s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > epsilon && r.get_t_min() < t && t < r.get_t_max()) // ray intersection
    {
        *t_hit = t;
        sf->p = r(t);
        sf->wo = glm::normalize(-r.get_direction());
        // Update the surface normal (which is normally normalized)
        sf->n = rt3::Lerp(v,rt3::Lerp(u, n0, n1),n2); // TODO: Check if this works
        sf->uv = Point2f(u, v);
        return true;
    }
    // This means that there is a line intersection but not a ray intersection.
    return false;

}
bool Triangle::intersect_p(const Ray &r) const{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    // This is how we retrieve the information associated with this particular triangle.
    const Point3f &p0 = mesh->vertices[v[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
    const Point3f &p1 = mesh->vertices[v[1]]; // Same for the 1-vertex.
    const Point3f &p2 = mesh->vertices[v[2]]; // Same for the 2-vertex.

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;
    Vector3f ray_cross_e2 = glm::cross(r.get_direction(), edge2);
    float det = glm::dot(edge1, ray_cross_e2);

    if (det > -epsilon && det < epsilon)
        return false;    // This ray is parallel to this triangle.

    float inv_det = 1.0 / det;
    Vector3f s = r.get_origin() - p0;
    float u = inv_det * dot(s, ray_cross_e2);

    if (u < 0 || u > 1)
        return false;

    Vector3f s_cross_e1 = glm::cross(s, edge1);
    float v = inv_det * glm::dot(r.get_direction(), s_cross_e1);

    if (v < 0 || u + v > 1)
        return false;

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = inv_det * dot(edge2, s_cross_e1);

    if (t > epsilon && r.get_t_min() < t && t < r.get_t_max()) // ray intersection
    {
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;

    return false;
}

Bounds3f Triangle::world_bounds(){

    const Point3f &p0 = mesh->vertices[v[0]]; // Get the 3D coordinate of the 0-vertex of this triangle.
    const Point3f &p1 = mesh->vertices[v[1]]; // Same for the 1-vertex.
    const Point3f &p2 = mesh->vertices[v[2]]; // Same for the 2-vertex.

    Point3f center = p0;

    real_type radius = std::max( glm::distance(p0, p1), glm::distance(p0, p2) );

    return Bounds3f(false, radius + 2, center);
}

std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals,
                                                     const ParamSet &ps){
    std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>();
    
    //TODO: These parameters aren't supported yet
    bool reverse_vertex_order = retrieve(ps, "reverse_vertex_order", false);
    bool compute_normals = retrieve(ps, "compute_normals", false);
    bool backface_cull = retrieve(ps, "backface_cull", false);
    if(ps.count("filename") >= 1){
        std::string filename = retrieve(ps, "filename", std::string("default.obj"));

        // Call our auxiliary function that interfaces with tinyobjloader API.
        if (load_mesh_data(filename, reverse_vertex_order, compute_normals,
                        flip_normals, mesh)) {
        std::cout << ">>> Mesh data successfuly loaded!\n";
        } else {
        std::cout << ">>> Mesh data loading failed!\n";
        }
    }
    else{

        int n_triangles = retrieve(ps, "ntriangles", 1);
        std::vector<int> indices = retrieve(ps, "indices", std::vector<int>{0,1,2});
        std::vector<Vector3f> vertices = retrieve(ps, "vertices", std::vector<Vector3f>({{-3, -0.5, -3}, {3, -0.5, -3}, {3, -0.5, 3}}));
        std::vector<Vector3f> normals = retrieve(ps, "normals", std::vector<Vector3f>({{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}));
        std::vector<Point2f> uv = retrieve(ps, "uv", std::vector<Point2f>({{0, 0}, {0, 1}, {1, 0}}));
        

        mesh->n_triangles = n_triangles;
        mesh->vertices = vertices;
        mesh->normals = normals;
        mesh->uvcoords = uv;
        mesh->normal_indices = indices;
        mesh->vertex_indices = indices;
        mesh->uvcoord_indices = indices;

    }


    return create_triangle_mesh(mesh, backface_cull, flip_normals );
}

std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh, bool bfc, bool fn){
    
    std::vector<std::shared_ptr<Shape>> shapes;
    std::shared_ptr<Shape> shape;
    int n_triangles = mesh->n_triangles;
    for(int i = 0; i < n_triangles; ++i){

        shape = std::shared_ptr<Shape>(new Triangle(mesh, i, bfc, fn));
        shapes.push_back(shape);
    }
    return shapes;
}

/// This function calls the basic tinyobjloader loading function and stores all
/// the data into the tinyobjloader's internal data structures.
bool load_mesh_data(const std::string &filename, bool rvo, bool cn, bool fn,
                    std::shared_ptr<TriangleMesh> md) {
  // Default load parameters
  const char *basepath = nullptr;
  bool triangulate = true;

  std::cout << "Loading " << filename << std::endl;

  // This is the tinyobj internal data structures.
  tinyobj::attrib_t
      attrib; // Hold all the data, such as vertex, normals, UV coords
  std::vector<tinyobj::shape_t>
      shapes; // Represents a collection of faces, grouped in a unit.
  std::vector<tinyobj::material_t> materials; // Materials, NOT USED HERE.

  // Warnings and Error values.
  std::string warn;
  std::string err;

  // Timing loading.
  //================================================================================
  auto start = std::chrono::steady_clock::now();
  //================================================================================
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                              filename.c_str(), basepath, triangulate);
  //================================================================================
  std::chrono::time_point<std::chrono::steady_clock> end =
      std::chrono::steady_clock::now();
  // Store the time difference between start and end
  std::chrono::duration<double> diff = end - start;

  // Milliseconds (10^-3)
  std::cout << "\t\t>>> "
            << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << std::endl;

  // Nanoseconds (10^-9)
  std::cout << "\t\t>>> "
            << std::chrono::duration<double, std::nano>(diff).count() << " ns"
            << std::endl;

  // Seconds
  auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
  std::cout << "\t\t>>> " << diff_sec.count() << " s" << std::endl;
  //================================================================================

  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    std::cerr << "ERR: " << err << std::endl;
  }

  if (!ret) {
    std::cout << "Failed to load/parse .obj.\n";
    return false;
  }

  // Let us now "convert" or "migrate" the data from tinyobjloader data
  // structure into our mesh data format.
  extract_obj_data(attrib, shapes, // TinyObjeLoader data structures (IN)
                   rvo, cn, fn,    // Mesh modifiers (IN)
                   md); // Reference to the mesh data to fill in. (OUT)

  return true;
}

/// This is the function that converts from the tinyobjloader's internal storage
/// to our mesh data organization.
void extract_obj_data(const tinyobj::attrib_t &attrib,
                      const std::vector<tinyobj::shape_t> &shapes,
                      bool reverse_order, bool cn, bool fn,
                      /* OUT */ std::shared_ptr<TriangleMesh> md) {
  std::cout << "-- SUMMARY of the OBJ file --\n";
  std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
            << std::endl;
  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "-----------------------------\n";

  // Retrieve the complete list of vertices.
  auto n_vertices{attrib.vertices.size() / 3};
  for (auto idx_v{0U}; idx_v < n_vertices; idx_v++) {
    // std::cout << "   v[" << static_cast<long>(idx_v) << "] = ( "
    //      << static_cast<double>(attrib.vertices[3 * idx_v + 0]) << ", "
    //      << static_cast<double>(attrib.vertices[3 * idx_v + 1]) << ", "
    //      << static_cast<double>(attrib.vertices[3 * idx_v + 2]) << " )\n";

    // Store the vertex in the mesh data structure.
    md->vertices.push_back(Point3f{attrib.vertices[3 * idx_v + 0],
                                   attrib.vertices[3 * idx_v + 1],
                                   attrib.vertices[3 * idx_v + 2]});
  }

  // Read the normals
  // TODO: flip normals if requested.
  real_type flip = (fn) ? -1 : 1;
  auto n_normals{attrib.normals.size() / 3};

  // Do we need to compute the normals? Yes only if the user requeste or there
  // are no normals in the file. if ( cn == true or n_normals == 0)
  // {
  //    // TODO: COmpute normals here.
  //    // compute_normals();
  // }
  // else {// Read normals from file. This corresponds to the entire 'for'
  // below.

  // Traverse the normals read from the OBJ file.
  for (auto idx_n{0U}; idx_n < n_normals; idx_n++) {
    // std::cout << "   n[" << static_cast<long>(idx_n) << "] = ( "
    //      << static_cast<double>(attrib.normals[3 * idx_n + 0]) << ", "
    //      << static_cast<double>(attrib.normals[3 * idx_n + 1]) << ", "
    //      << static_cast<double>(attrib.normals[3 * idx_n + 2]) << " )\n";

    // Store the normal.
    md->normals.push_back(Vector3f{attrib.normals[3 * idx_n + 0] * flip,
                                   attrib.normals[3 * idx_n + 1] * flip,
                                   attrib.normals[3 * idx_n + 2] * flip});
  }

  // Read the complete list of texture coordinates.
  auto n_texcoords{attrib.texcoords.size() / 2};
  for (auto idx_tc{0U}; idx_tc < n_texcoords; idx_tc++) {
    // std::cout << "   t[" << static_cast<long>(idx_tc) << "] = ( "
    //      << static_cast<double>(attrib.texcoords[2 * idx_tc + 0]) << ", "
    //      << static_cast<double>(attrib.texcoords[2 * idx_tc + 1]) << " )\n";

    // Store the texture coords.
    md->uvcoords.push_back(Point2f{attrib.texcoords[2 * idx_tc + 0],
                                   attrib.texcoords[2 * idx_tc + 1]});
  }

  // Read mesh connectivity and store it as lists of indices to the real data.
  auto n_shapes{shapes.size()};
  md->n_triangles = 0; // We must reset this here.
  // In case the OBJ file has the triangles organized in several shapes or
  // groups, we ignore this and store all triangles as a single mesh dataset.
  // This is why we need to reset the triangle count here.
  for (auto idx_s{0U}; idx_s < n_shapes; idx_s++) {
    // std::cout << "The shape[" << idx_s << "].name = " << shapes[idx_s].name << '\n';
    // std::cout << "Size of shape[" << idx_s << "].mesh.indices: "
    //      << static_cast<unsigned long>(shapes[idx_s].mesh.indices.size())
    //      << '\n';
    // std::cout << "shape[" << idx_s << "].num_faces: "
    //      << static_cast<unsigned long>(
    //             shapes[idx_s].mesh.num_face_vertices.size())
    //      << '\n';

    // For each face print out the indices lists.
    size_t index_offset = 0;
    // # of triangles for this "shape" (group).
    // NOTE that we are accumulate the number of triangles coming from the
    // shapes present in the OBJ file.
    md->n_triangles += shapes[idx_s].mesh.num_face_vertices.size();
    for (auto idx_f{0}; idx_f < md->n_triangles; idx_f++) {
      // Number of vertices per face (always 3, in our case)
      size_t fnum = shapes[idx_s].mesh.num_face_vertices[idx_f];

      // std::cout << "  face[" << idx_f
      //      << "].fnum = " << static_cast<unsigned long>(fnum) << '\n';

      // TODO: Invert order of vertices if flag is on. (DONE!)
      if (reverse_order) {
        // std::cout << "Reverse order\n";
        // For each vertex in the face print the corresponding indices
        for (int v = fnum - 1; v >= 0; v--) {
          tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
          // std::cout << "    face[" << idx_f << "].v[" << v
          //      << "].indices = " << idx.vertex_index << "/" << idx.normal_index
          //      << "/" << idx.texcoord_index << '\n';
          // Add the indices to the global list of indices we need to pass on to
          // the mesh object.
          md->vertex_indices.push_back(idx.vertex_index);
          md->normal_indices.push_back(idx.normal_index);
          md->uvcoord_indices.push_back(idx.texcoord_index);
        }
      } else { // Keep the original vertex order
        // For each vertex in the face get the corresponding indices
        for (size_t v = 0; v < fnum; v++) {
          tinyobj::index_t idx = shapes[idx_s].mesh.indices[index_offset + v];
          // std::cout << "    face[" << idx_f << "].v[" << v
          //      << "].indices = " << idx.vertex_index << "/" << idx.normal_index
          //      << "/" << idx.texcoord_index << '\n';
          // Add the indices to the global list of indices we need to pass on to
          // the mesh object. This goes to the mesh data structure.
          md->vertex_indices.push_back(idx.vertex_index);
          md->normal_indices.push_back(idx.normal_index);
          md->uvcoord_indices.push_back(idx.texcoord_index);
        }
      }

      // Advance over to the next triangle.
      index_offset += fnum;
    }
  }

  // std::cout << "This is the list of indices: \n";

  // std::cout << "   + Vertices [ ";
  // std::copy(md->vertex_indices.begin(), md->vertex_indices.end(),
  //           std::ostream_iterator<int>(std::cout, " "));
  // std::cout << "]\n";

  // std::cout << "   + Normals [ ";
  // std::copy(md->normal_indices.begin(), md->normal_indices.end(),
  //           std::ostream_iterator<int>(std::cout, " "));
  // std::cout << "]\n";

  // std::cout << "   + UV coords [ ";
  // std::copy(md->uvcoord_indices.begin(), md->uvcoord_indices.end(),
  //           std::ostream_iterator<int>(std::cout, " "));
  // std::cout << "]\n";
}

} // namespace rt3