#ifndef _DIELECTRIC_H_
#define _DIELECTRIC_H_

#include "Material.h"

#include "Util.h"

// 电介质，如水、玻璃,非金属材质 = 反射+折射,金属材质 = (反射)[漫反射 + 镜面反射]
class DieletricMaterial :public Material {
public:
	DieletricMaterial(float ior) :ior(ior) {}

public:
	static Ptr<DieletricMaterial> New(float ior) {
		return std::make_shared<DieletricMaterial>(ior);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	float ior;
};

// 反射和折射一起递归计算
const ScatterRst DieletricMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f I = ray.d.Normalize();

	Vec3f R; // 折射方向
	if (!Util::Refract(I, rec.n, ior, R)) { // 如果是全反射,则百分百返回颜色
		// 全反射
		Vec3f dir = Util::Reflect(I, rec.n);
		return ScatterRst(true, { rec.p, dir }, Vec3f(1.f));
	}

	float IoN = I.Dot(rec.n);
	bool isEntering = IoN < 0;
	// 必须是空气中的方向与法向的夹角的余弦
	float cosTheta = isEntering ? -I.Dot(rec.n) : R.Dot(rec.n);
	float F = Util::Fresnel_Schlick(ior, cosTheta); // 菲涅尔系数，即反射概率

	if (Util::RandF() < F) {
		// 反射
		Vec3f dir = Util::Reflect(I, rec.n);
		return ScatterRst(true, { rec.p, dir }, Vec3f(1.f));
	}

	// 折射
	return ScatterRst(true, { rec.p, R }, Vec3f(1.f));
}

#endif // !_DIELECTRIC_H_
