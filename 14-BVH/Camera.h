#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Vec3.h"
#include "Ray.h"
#include "Util.h"

class Camera {
public:
	// vfov: verticle field of view，纵向的角度
	// aspect: width / height，宽高比
	// aperture: 透镜直径
	// focusDis: 对焦距离，就是最清晰处离相机的距离
	Camera(const Vec3f & pos, const Vec3f & target, float vfov, float aspect,
		
		float aperture = 0.f, float focusDis = 1.f)
	{
		Init(pos, target, vfov, aspect, aperture, focusDis);
	}

public:
	void Init(const Vec3f & pos, const Vec3f & target, float vfov, float aspect,		
		float aperture = 0.f, float focusDis = 1.f);

public:
	const Ray GenRay(float u, float v) const;

private:
	Vec3f pos; // 相机位置
	Vec3f lowerLeft; // 成像平面的左下角

	Vec3f u; // 单位向量
	Vec3f horizontal; // 长度为成像平面宽度

	Vec3f v; // 单位向量
	Vec3f vertical; // 长度为成像平面高度

	
	float lensRadius; // 透镜半径
};

void Camera::Init(const Vec3f & pos, const Vec3f & target, float vfov, float aspect,
	
	float aperture, float focusDis)
{
	this->pos = pos;

	float theta = vfov / 180.f * Util::PI; // 角度转弧度
	float halfHeight = focusDis * tan(theta / 2.f); // 成像平面高度的一半
	float halfWidth = aspect * halfHeight; // 成像平面宽度的一半

	lensRadius = aperture / 2.f;
	// Opengl相机朝向为-Z方向
	Vec3f w = (pos - target).Normalize(); // -dir,camera faced -Z, camera faces -w
	const Vec3f worldUp(0, 1, 0);
	u = worldUp.Cross(w);
	v = w.Cross(u);
	// 相机坐标系下uvw,从相机位置，移动到屏幕左下角，移动三个轴uvw
	lowerLeft = pos - focusDis * w - halfWidth * u - halfHeight * v;
	horizontal = 2.f * halfWidth * u;
	vertical = 2.f * halfHeight * v;
}

// dir = (target-cameraPos).Normalize()
const Ray Camera::GenRay(float _u, float _v) const {
	// 当 lensRadius == 0 时退化为
	// {pos, lowerLeft + u * horizontal + v * vertical - pos}

	Vec3f target = lowerLeft + _u * horizontal + _v * vertical;

	Vec3f rd = lensRadius * Util::RandInDisk();
	Vec3f offset = u * rd.x + v * rd.y;
	Vec3f origin = pos + offset;

	return { origin,  target - origin };
}

#endif // !_CAMERA_H_
