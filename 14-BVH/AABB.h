#ifndef AABBH
#define AABBH

#include "Vec3.h"
#include "Hitable.h"

class AABB {
public:
	AABB(const Vec3f & minP = Vec3f(Util::LARGE_FLT), const Vec3f & maxP = Vec3f(-Util::LARGE_FLT))
		: minP(minP), maxP(maxP) { }

public:
	struct HitRst {
		bool hit;
		float t0;// min ������Сֵ
		float t1;// max �������ֵ
	};
	const HitRst Hit(const Ray & ray) const;
	const bool hit(const Ray &ray) const;
	// �ϲ���������
	static const AABB UnionAABB(const AABB & lhs, const AABB & rhs);	
	const AABB UnionWith(const AABB & rhs) const {
		return UnionAABB(*this, rhs);
	}

	const Vec3f Center() const { return (minP + maxP) / 2.f; }

public:
	Vec3f minP;
	Vec3f maxP;
};

const bool AABB::hit(const Ray &ray) const
{
	float tMin = ray.tMin;
	float tMax = ray.tMax;
	auto invDir = ray.InvDir();

	//HitRst rst;

	for (int axis = 0; axis < 3; axis++) {
		float invD = invDir[axis];
		float t0 = (minP[axis] - ray.o[axis]) * invD;
		float t1 = (maxP[axis] - ray.o[axis]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);
		// ��Χ��
		tMin = (std::max)(t0, tMin);
		tMax = (std::min)(t1, tMax);
		if (tMax < tMin) {
			
			return false;
		}
	}
	//rst.hit = true;
	//rst.t0 = tMin;
	//rst.t1 = tMax;
	return true;
}
// ��aabb box�������߼�⣬ʹ�� slap���߼�ⷽ��
const AABB::HitRst AABB::Hit(const Ray & ray) const {
	float tMin = ray.tMin;
	float tMax = ray.tMax;
	auto invDir = ray.InvDir();

	HitRst rst;

	for (int axis = 0; axis < 3; axis++) {
		float invD = invDir[axis];
		float t0 = (minP[axis] - ray.o[axis]) * invD;
		float t1 = (maxP[axis] - ray.o[axis]) * invD;
		if (invD < 0.0f)
			std::swap(t0, t1);
		// ��Χ��
		tMin = (std::max)(t0, tMin);
		tMax = (std::min)(t1, tMax);
		if (tMax < tMin) {
			rst.hit = false;
			return rst;
		}
	}
	rst.hit = true;
	rst.t0 = tMin;
	rst.t1 = tMax;
	return rst;
}
// �ϲ�2��aabb
const AABB AABB::UnionAABB(const AABB & lhs, const AABB & rhs) {
	auto minP = Vec3f::Min(lhs.minP, rhs.minP);
	auto maxP = Vec3f::Max(lhs.maxP, rhs.maxP);
	return { minP, maxP };
}


#endif