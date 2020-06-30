#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "Ray.h"
#include "AABB.h"

#include <memory>

template<typename T>
using Ptr = std::shared_ptr<T>;


class Material; // ǰ������

struct HitRecord {
	// float t; ������ Ray �м�¼ tMin �� tMax
	Vec3f p; // ������ײ��
	Vec3f n; // ��λ����
	Ptr<Material> material;
};

class Hitable {
public:
	// ��Ϊ Ray �к��� tMin �� tMax���������ﲻ��Ҫ������Ӧ����
	// ����ཻ���������޸� ray �� tMax
	virtual bool Hit(Ray & ray, HitRecord & rec) const = 0;
	virtual const AABB GetBox() const = 0;
	
};

#endif // !_HITABLE_H_
