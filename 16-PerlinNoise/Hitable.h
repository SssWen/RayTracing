#ifndef _HITABLE_H_
#define _HITABLE_H_

#include "Ray.h"
#include "AABB.h"

#include <memory>
#include <math.h> 

template<typename T>
using Ptr = std::shared_ptr<T>;


class Material; // 前向声明

struct HitRecord {
	// float t; 我们在 Ray 中记录 tMin 和 tMax
	Vec3f p; // 射线碰撞点
	Vec3f n; // 单位法向
	Ptr<Material> material;

	float u;
	float v;
};

// 在对应的碰撞物体内算uv
// 球面坐标系(r,theta,phi),面上一点p(x,y,z),计算UV, 
// 根据v->theta[-π/2,π/2]和u->phi[-π,π]的范围映射uv
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
	// 因为 Ray 中含有 tMin 和 tMax，所以这里不需要输入相应参数
	// 如果相交，函数会修改 ray 的 tMax
	// 碰撞到后,修改射线的起点和方向,并记录碰撞到的材质
	virtual bool Hit(Ray & ray, HitRecord & rec) const = 0;
	virtual const AABB GetBox() const = 0;
	
};

#endif // !_HITABLE_H_
