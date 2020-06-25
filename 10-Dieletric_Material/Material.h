#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Hitable.h"

// ���ʵ�ɢ��,diffuse ʹ��albedo����attenuation,
struct ScatterRst {
	ScatterRst(bool isScatter, const Ray & ray, const Vec3f albedo) :
		isScatter(isScatter),
		ray(ray),
		attenuation(albedo) { }

	// ���߱���ȫ���յ����,������뷨���෴
	static const ScatterRst & InValid() {
		static ScatterRst invalid(false, Ray(Vec3f(0.f), Vec3f(0.f)), Vec3f(0.f));
		return invalid;
	}

	bool isScatter;
	Ray ray; // �µĹ���
	Vec3f attenuation; // ˥��
};

// ������
class Material {
public:
	virtual const ScatterRst Scatter(const Ray & ray, const HitRecord & rec) const = 0;		
	virtual const Vec3f Emit() const { return 0; } 
};

#endif // !_MATERIAL_H_