cmake_minimum_required(VERSION 3.5)
project (BasicRT3 VERSION 1.0.0 LANGUAGES CXX )

#=== CREATING COMPILING DATABASE ===#
# Currently CMake (since 2.8.5) supports generation of compilation databases
# for Unix Makefile builds (Ninja builds in the works) with the option
# CMAKE_EXPORT_COMPILE_COMMANDS.
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

#=== FINDING PACKAGES ===#

# # find_package(TinyXml2 REQUIRED)

# Set "manually" paths that need to be considered while compiling/linking
include_directories( cameras
                     integrators
                     backgrounds
                     core
                     materials
                     shapes
                     primitives
                     lights
                     ext )

#=== SETTING VARIABLES ===#
# Compiling flags
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -pedantic -O3 -ffast-math" )
set( RT3_SOURCE_DIR "src" )

#=== main  target ===
add_executable(basic_rt3 ${RT3_SOURCE_DIR}/backgrounds/background.cpp
                         ${RT3_SOURCE_DIR}/shapes/shape.cpp
                         ${RT3_SOURCE_DIR}/cameras/camera.cpp
                         ${RT3_SOURCE_DIR}/cameras/film.cpp
                         ${RT3_SOURCE_DIR}/core/api.cpp
                         ${RT3_SOURCE_DIR}/core/error.cpp
                         ${RT3_SOURCE_DIR}/core/image_io.cpp
                         ${RT3_SOURCE_DIR}/core/parser.cpp
                         ${RT3_SOURCE_DIR}/core/ray.cpp
                         ${RT3_SOURCE_DIR}/integrators/integrator.cpp
                         ${RT3_SOURCE_DIR}/integrators/blinn_phong_integrator.cpp
                         ${RT3_SOURCE_DIR}/lights/ambient_light.cpp
                         ${RT3_SOURCE_DIR}/lights/directional_light.cpp
                         ${RT3_SOURCE_DIR}/lights/light.cpp
                         ${RT3_SOURCE_DIR}/lights/point_light.cpp
                         ${RT3_SOURCE_DIR}/lights/spot_light.cpp
                         ${RT3_SOURCE_DIR}/materials/material.cpp
                         ${RT3_SOURCE_DIR}/primitives/bounds.cpp
                         ${RT3_SOURCE_DIR}/primitives/BVHAccel.cpp
                         ${RT3_SOURCE_DIR}/primitives/primitive.cpp
                         ${RT3_SOURCE_DIR}/primitives/primlist.cpp
                         ${RT3_SOURCE_DIR}/shapes/plane.cpp
                         ${RT3_SOURCE_DIR}/shapes/simple_triangle.cpp
                         ${RT3_SOURCE_DIR}/shapes/triangle.cpp
                         ${RT3_SOURCE_DIR}/shapes/sphere.cpp
                         ${RT3_SOURCE_DIR}/ext/lodepng.cpp
                         ${RT3_SOURCE_DIR}/ext/tinyxml2.cpp
                         ${RT3_SOURCE_DIR}/ext/tiny_obj_loader.cc
                         ${RT3_SOURCE_DIR}/main/rt3.cpp
                        )

#define C++17 as the standard.
set_property(TARGET basic_rt3 PROPERTY CXX_STANDARD 17)
