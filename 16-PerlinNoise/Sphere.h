#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Hitable.h"
#include "Material.h"

class Sphere : public Hitable {
public:
	Sphere(const Vec3f & center, float radius, Ptr<Material> material)
		: center(center), radius(radius), material(material){ }

public:
	// 生成 Ptr 的便利接口
	static Ptr<Sphere> New(const Vec3f & center, float radius, Ptr<Material> material = nullptr) {
		return std::make_shared<Sphere>(center, radius, material);
	}
	virtual const AABB GetBox() const override {
		return { center - Vec3f(radius), center + Vec3f(radius) };
	}

public:
	virtual bool Hit(Ray & ray, HitRecord & rec) const override;


public:
	Vec3f center;
	float radius;
	Ptr<Material> material;
};

bool Sphere::Hit(Ray & ray, HitRecord & rec) const {
	// o + t * d == p
	// (p-c)^2 == r^2
	// (d * t + o - c)^2 == r^2
	// d*d * t^2 + 2*d*(o-c) * t + (o-c)^2 - r^2 = 0

	auto oc = ray.o - center;

	float a = ray.d.Dot(ray.d);
	float b = ray.d.Dot(oc); 
	float c = oc.Dot(oc) - radius * radius;

	float discriminant = b * b - a * c; // 化简
	if (discriminant < 0.f) // 没有交点
		return false;
	float sqrtDiscriminant = sqrt(discriminant);
	float t0 = (-b - sqrtDiscriminant) / a; // 较小根
	if (t0 < ray.tMax && t0 > ray.tMin)
	{
		ray.tMax = t0;
		rec.p = ray.EndPos();
		rec.n = (rec.p - center) / radius;
		rec.material = material;
		get_sphere_uv(rec.p - center, rec.u, rec.v);
		return true;
	}
	t0 = (-b - sqrtDiscriminant) / a; // 较大根
	if (t0 < ray.tMax && t0 > ray.tMin)
	{
		ray.tMax = t0;
		rec.p = ray.EndPos();
		rec.n = (rec.p - center) / radius;// 法线 = normalize(碰撞点-球心)
		rec.material = material;
		get_sphere_uv(rec.p - center, rec.u, rec.v);
		return true;
	}
	return false;
}

#endif // !_SPHERE_H_
