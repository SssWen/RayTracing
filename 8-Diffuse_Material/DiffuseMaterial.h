#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"

#include "Util.h"

// 漫反射，朗伯模型
class DiffuseMaterial : public Material {
public:
	DiffuseMaterial(const Vec3f & _albedo) : albedo(_albedo) { }

public:
	// 生成 Ptr 的便利接口
	static Ptr<DiffuseMaterial> New(const Vec3f & albedo) {
		return std::make_shared<DiffuseMaterial>(albedo);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	Vec3f albedo; // 反射率，即颜色
};

const ScatterRst DiffuseMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = rec.n + Util::RandInSphere(); // 模拟漫反射
	Ray scatterRay(rec.p, dir.Normalize());
	return ScatterRst(true, scatterRay, albedo); // 新Ray,衰减参数
}

#endif // !_LAMBERTIAN_H_
