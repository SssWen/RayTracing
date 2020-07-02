#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Util.h"
#include "Texture.h"

// 漫反射，朗伯模型
class DiffuseMaterial : public Material {
public:	
	DiffuseMaterial(const std::shared_ptr<Texture>_albedo) : albedo(_albedo) { }

public:
	// 生成 Ptr 的便利接口
	static Ptr<DiffuseMaterial> New(const std::shared_ptr<Texture> albedo) {
		return std::make_shared<DiffuseMaterial>(albedo);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:	
	const std::shared_ptr<Texture> albedo; // 反射率，即颜色
};

const ScatterRst DiffuseMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = rec.n + Util::RandInSphere(); // 模拟漫反射
	Ray scatterRay(rec.p, dir.Normalize());
	// 通过albedo Texture 获取颜色值
	return ScatterRst(true, scatterRay, albedo->value(rec.u,rec.v,rec.p)); // 新Ray,衰减参数
}

#endif // !_LAMBERTIAN_H_
