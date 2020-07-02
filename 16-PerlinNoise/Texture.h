#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "Vec3.h"

class Texture {
public:
	virtual Vec3f value(float u, float v, const Vec3f& p) const = 0;
};

// 纯色纹理
class Constant_Texture :public Texture{
public:
	Constant_Texture() {}
	Constant_Texture(Vec3f c) : color(c) {}
	virtual Vec3f value(float u, float v, const Vec3f& p) const {
		return color; // diffuse 直接返回纯色
	}
	Vec3f color;
};

// 棋盘纹理
class Check_Texture : public Texture {
public:
	Check_Texture() {}
	Check_Texture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1) : even(t0), odd(t1) {}
	virtual Vec3f value(float u, float v, const Vec3f& p) const {
		float sines = sin(10 * p.x)*sin(10 * p.y)*sin(10 * p.z);
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
	/*Texture *odd;
	Texture *even;*/
	std::shared_ptr<Texture> odd;
	std::shared_ptr<Texture> even;

};


#endif 
