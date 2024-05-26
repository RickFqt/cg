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

    if (t > epsilon && t < r.get_t_max()) // ray intersection
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

    if (t > epsilon && t < r.get_t_max()) // ray intersection
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
    auto mesh = new TriangleMesh;
    
    // TODO: Ver isso dai ta ligado
    bool reverse_vertex_order = retrieve(ps, "reverse_vertex_order", false);
    bool compute_normals = retrieve(ps, "compute_normals", false);
    bool backface_cull = retrieve(ps, "backface_cull", false);
    if(ps.count("filename") >= 1){
        std::string filename = retrieve(ps, "filename", std::string("default.obj"));

        // TODO
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


    return create_triangle_mesh(std::shared_ptr<TriangleMesh>(mesh), backface_cull, flip_normals );
}

std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh> mesh, bool bfc, bool fn){
    
    std::vector<std::shared_ptr<Shape>> shapes;
    std::shared_ptr<Shape> shape;
    //TODO
    int n_triangles = mesh->n_triangles;
    for(int i = 0; i < n_triangles; ++i){

        shape = std::shared_ptr<Shape>(new Triangle(mesh, i, bfc, fn));
        shapes.push_back(shape);
    }
    return shapes;
}

} // namespace rt3