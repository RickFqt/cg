#include "shape.h"

namespace rt3{

    Shape::Shape(bool flip_n, const Transform & obj_to_world):flip_normals{flip_n}, obj_to_world{new Transform(obj_to_world.getMatrix())}{
        world_to_obj = new Transform(obj_to_world.getInverseMatrix());
    }
}