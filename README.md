# Intro

This is the basic architecture for the Ray Tracing Teaching Tool (RT3) Project.

# Processing flow

The `main.cpp` calls the `api.cpp`, which, in turn, calls various API functions that create the objects, such as camera, integrator, scene, film.
The information to create these classes are stored in the `RenderOptions` struct, while the scene is being parsed.

When the parser finds the tag `world_end` it creates all the objects (film, camera, scene, integrator) and calls the `render()` method.
This method corresponds to the "main loop" of the rendering process.

# To compile

```
cmake -S . -B build
cmake --build build
./build/basic_rt3 scene/scene_01.xml
```

# TODO

- [ ] Rendering multiple files at once
