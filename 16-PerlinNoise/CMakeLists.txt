project(16-PerlinNoise VERSION 1.0.0)

add_executable(16-PerlinNoise
	main.cpp
	Vec3.h
	Ray.h
	Hitable.h
	HitableList.h
	Sphere.h
	Material.h
	DiffuseMaterial.h
	MetalMaterial.h
	DieletricMaterial.h
	Camera.h
	Util.h
	BVHNode.h
	AABB.h
	Texture.h
)