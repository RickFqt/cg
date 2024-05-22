#include "triangle.h"

namespace rt3 {

bool Triangle::intersect(const Ray &r, float *t_hit, Surfel *sf){

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
        sf->n = n0; // TODO: Change this to interpolation of each vertex normal
        sf->uv = Point2f(u, v);
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;

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

std::vector<std::shared_ptr<Shape>> create_triangle_mesh_shape(bool flip_normals,
                                                     const ParamSet &ps){
    std::vector<std::shared_ptr<Shape>> shapes;
    //TODO
    return shapes;
}

std::vector<std::shared_ptr<Shape>> create_triangle_mesh(std::shared_ptr<TriangleMesh>, bool){
    
    std::vector<std::shared_ptr<Shape>> shapes;
    //TODO
    return shapes;
}

} // namespace rt3