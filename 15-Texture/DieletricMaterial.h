#ifndef _DIELECTRIC_H_
#define _DIELECTRIC_H_

#include "Material.h"

#include "Util.h"

// ����ʣ���ˮ������,�ǽ������� = ����+����,�������� = (����)[������ + ���淴��]
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

// ���������һ��ݹ����
const ScatterRst DieletricMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f I = ray.d.Normalize();

	Vec3f R; // ���䷽��
	if (!Util::Refract(I, rec.n, ior, R)) { // �����ȫ����,��ٷְٷ�����ɫ
		// ȫ����
		Vec3f dir = Util::Reflect(I, rec.n);
		return ScatterRst(true, { rec.p, dir }, Vec3f(1.f));
	}

	float IoN = I.Dot(rec.n);
	bool isEntering = IoN < 0;
	// �����ǿ����еķ����뷨��ļнǵ�����
	float cosTheta = isEntering ? -I.Dot(rec.n) : R.Dot(rec.n);
	float F = Util::Fresnel_Schlick(ior, cosTheta); // ������ϵ�������������

	if (Util::RandF() < F) {
		// ����
		Vec3f dir = Util::Reflect(I, rec.n);
		return ScatterRst(true, { rec.p, dir }, Vec3f(1.f));
	}

	// ����
	return ScatterRst(true, { rec.p, R }, Vec3f(1.f));
}

#endif // !_DIELECTRIC_H_
