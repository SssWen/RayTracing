#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "Ray.h"
#include "AABB.h"

#include <memory>

template<typename T>
using Ptr = std::shared_ptr<T>;


class Material; // 前向声明

struct HitRecord {
	// float t; 我们在 Ray 中记录 tMin 和 tMax
	Vec3f p; // 射线碰撞点
	Vec3f n; // 单位法向
	Ptr<Material> material;
};

class Hitable {
public:
	// 因为 Ray 中含有 tMin 和 tMax，所以这里不需要输入相应参数
	// 如果相交，函数会修改 ray 的 tMax
	virtual bool Hit(Ray & ray, HitRecord & rec) const = 0;
	virtual const AABB GetBox() const = 0;
	
};

#endif // !_HITABLE_H_
