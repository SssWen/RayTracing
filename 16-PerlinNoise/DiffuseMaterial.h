#ifndef _LAMBERTIAN_H_
#define _LAMBERTIAN_H_

#include "Material.h"
#include "Util.h"
#include "Texture.h"

// �����䣬�ʲ�ģ��
class DiffuseMaterial : public Material {
public:	
	DiffuseMaterial(const std::shared_ptr<Texture>_albedo) : albedo(_albedo) { }

public:
	// ���� Ptr �ı����ӿ�
	static Ptr<DiffuseMaterial> New(const std::shared_ptr<Texture> albedo) {
		return std::make_shared<DiffuseMaterial>(albedo);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:	
	const std::shared_ptr<Texture> albedo; // �����ʣ�����ɫ
};

const ScatterRst DiffuseMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	Vec3f dir = rec.n + Util::RandInSphere(); // ģ��������
	Ray scatterRay(rec.p, dir.Normalize());
	// ͨ��albedo Texture ��ȡ��ɫֵ
	return ScatterRst(true, scatterRay, albedo->value(rec.u,rec.v,rec.p)); // ��Ray,˥������
}

#endif // !_LAMBERTIAN_H_
