#ifndef _METAL_H_
#define _METAL_H_

#include "Material.h"
#include "Util.h"

// 金属
class MetalMaterial : public Material {
public:
	MetalMaterial(const Vec3f & albedo, float fuzz) : albedo(albedo), fuzz(fuzz) { }

public:
	// 生成 Ptr 的便利接口
	static Ptr<MetalMaterial> New(const Vec3f & albedo, float fuzz = 0.f) {
		return std::make_shared<MetalMaterial>(albedo, fuzz);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	Vec3f albedo; // 反射率，即颜色
	float fuzz; // 0 -- 1
};

const ScatterRst MetalMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	//Vec3f dir = rec.n + Util::RandInSphere(); // 模拟漫反射
	// 模拟镜面反射,反射向量 + 随机偏移（为了模拟粗糙的金属表面）fuzz为随机偏移半径
	Vec3f dir = Util::Reflect(ray.d.Normalize(), rec.n) + fuzz * Util::RandInSphere();
	if (dir.Dot(rec.n) <= 0) // 表面内侧
		return ScatterRst::InValid();

	Ray newRay(rec.p, dir); // 碰撞点+反射方向 继续进行 反射
	return ScatterRst(true, newRay, albedo); // 用反射率当作衰减
}

#endif // !_METAL_H_
