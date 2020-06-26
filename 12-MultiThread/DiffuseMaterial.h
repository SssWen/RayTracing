#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"

#include "Util.h"

// �����䣬�ʲ�ģ��
class DiffuseMaterial : public Material {
public:
	DiffuseMaterial(const Vec3f & _albedo) : albedo(_albedo) { }

public:
	// ���� Ptr �ı����ӿ�
	static Ptr<DiffuseMaterial> New(const Vec3f & albedo) {
		return std::make_shared<DiffuseMaterial>(albedo);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	Vec3f albedo; // �����ʣ�����ɫ
};

const ScatterRst DiffuseMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = rec.n + Util::RandInSphere(); // ģ��������
	Ray scatterRay(rec.p, dir.Normalize());
	return ScatterRst(true, scatterRay, albedo); // ��Ray,˥������
}

#endif // !_LAMBERTIAN_H_
