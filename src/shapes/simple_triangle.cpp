#include "simple_triangle.h"

namespace rt3 {

SimpleTriangle::SimpleTriangle(const bool& flip_n, const Point3f& p0, const Point3f& p1, const Point3f& p2, const Vector3f& n):
Shape(flip_n), p0{p0}, p1{p1}, p2{p2}, norm{n}
{}

SimpleTriangle::SimpleTriangle(const bool& flip_n, const Point3f& po0, const Point3f& po1, const Point3f& po2):
Shape(flip_n), p0{po0}, p1{po1}, p2{po2}, norm{glm::normalize( glm::cross(po1 - po0, po2 - po0))}
{}

bool SimpleTriangle::intersect_p( const Ray& r ) const{

    

    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p1;
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

bool SimpleTriangle::intersect( const Ray& r, float *t_hit, Surfel *sf ) const{
    
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p1;
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
        sf->n = norm; // TODO: Change this to interpolation of each vertex normal
        sf->uv = Point2f(u, v);
        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;

    return false;

}

// Bounds3f Triangle::world_bounds(){

//     return Bounds3f(center - radius - 2, center + radius + 2);
// }

Bounds3f SimpleTriangle::world_bounds(){

    real_type min_x, min_y, min_z, max_x, max_y, max_z;

    min_x = std::min(std::min(p0.x, p1.x) , p2.x);
    min_y = std::min(std::min(p0.y, p1.y) , p2.y);
    min_z = std::min(std::min(p0.z, p1.z) , p2.z);

    max_x = std::max(std::max(p0.x, p1.x) , p2.x);
    max_y = std::max(std::max(p0.y, p1.y) , p2.y);
    max_z = std::max(std::max(p0.z, p1.z) , p2.z);

    return Bounds3f({min_x, min_y, min_z}, {max_x, max_y, max_z});
}

// Factory function pattern.
// This is the function that retrieves from the ParamSet object
// all the information we need to create a Triangle object.
SimpleTriangle* create_simple_triangle(const ParamSet &ps){
    
    Point3f p0 = retrieve(ps, "p0", Point3f{1,0,0});
    Point3f p1 = retrieve(ps, "p1", Point3f{0,1,0});
    Point3f p2 = retrieve(ps, "p2", Point3f{0,0,1});

    if(ps.count("normal") >= 1){
        Vector3f norm = retrieve(ps, "normal", Vector3f{0,0,1});
        return new SimpleTriangle(false, p0, p1, p2, norm);
    }

    // TODO: Add flip_normals
    return new SimpleTriangle(false, p0, p1, p2);
}


}  // namespace rt3
