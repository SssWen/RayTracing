#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "Ray.h"
#include "AABB.h"

#include <memory>
#include <math.h> 

template<typename T>
using Ptr = std::shared_ptr<T>;


class Material; // ǰ������

struct HitRecord {
	// float t; ������ Ray �м�¼ tMin �� tMax
	Vec3f p; // ������ײ��
	Vec3f n; // ��λ����
	Ptr<Material> material;

	float u;
	float v;
};

// �ڶ�Ӧ����ײ��������uv
// ��������ϵ(r,theta,phi),����һ��p(x,y,z),����UV, 
// ����v->theta[-��/2,��/2]��u->phi[-��,��]�ķ�Χӳ��uv
void get_sphere_uv(const Vec3f& p, float& u, float& v) {
	float phi = atan2(p.z, p.x);
	float theta = asin(p.y);
	u = 1 - (phi + Util::PI) / (2 * Util::PI);
	v = (theta + Util::PI / 2) / Util::PI;
	/*u = phi / (2 * Util::PI);	
	v = theta / Util::PI;*/
}

class Hitable {
public:
	// ��Ϊ Ray �к��� tMin �� tMax���������ﲻ��Ҫ������Ӧ����
	// ����ཻ���������޸� ray �� tMax
	// ��ײ����,�޸����ߵ����ͷ���,����¼��ײ���Ĳ���
	virtual bool Hit(Ray & ray, HitRecord & rec) const = 0;
	virtual const AABB GetBox() const = 0;
	
};

#endif // !_HITABLE_H_
