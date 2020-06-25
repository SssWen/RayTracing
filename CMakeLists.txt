cmake_minimum_required(VERSION 3.5)

project(RayTracing VERSION 1.0.0)
cmake_policy(VERSION 3.5)

add_subdirectory(1-OutputImage)
add_subdirectory(2-Vec3)
add_subdirectory(3-RayAndCamera)
add_subdirectory(4-AddingSphere)
add_subdirectory(5-SurfaceNormal)
add_subdirectory(6-Antialiasing)
add_subdirectory(7-Diffuse_GammaCorrect)
add_subdirectory(8-Diffuse_Material)
add_subdirectory(9-Metal_Material)
add_subdirectory(10-Dieletric_Material)

#add_subdirectory(library)
