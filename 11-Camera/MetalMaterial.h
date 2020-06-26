#ifndef _METAL_H_
#define _METAL_H_

#include "Material.h"
#include "Util.h"

// ����
class MetalMaterial : public Material {
public:
	MetalMaterial(const Vec3f & albedo, float fuzz) : albedo(albedo), fuzz(fuzz) { }

public:
	// ���� Ptr �ı����ӿ�
	static Ptr<MetalMaterial> New(const Vec3f & albedo, float fuzz = 0.f) {
		return std::make_shared<MetalMaterial>(albedo, fuzz);
	}

public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const override;

public:
	Vec3f albedo; // �����ʣ�����ɫ
	float fuzz; // 0 -- 1
};

const ScatterRst MetalMaterial::Scatter(const Ray & ray, const HitRecord & rec) const {
	//Vec3f dir = rec.n + Util::RandInSphere(); // ģ��������
	// ģ�⾵�淴��,�������� + ���ƫ�ƣ�Ϊ��ģ��ֲڵĽ������棩fuzzΪ���ƫ�ư뾶
	Vec3f dir = Util::Reflect(ray.d.Normalize(), rec.n) + fuzz * Util::RandInSphere();
	if (dir.Dot(rec.n) <= 0) // �����ڲ�
		return ScatterRst::InValid();

	Ray newRay(rec.p, dir); // ��ײ��+���䷽�� �������� ����
	return ScatterRst(true, newRay, albedo); // �÷����ʵ���˥��
}

#endif // !_METAL_H_
