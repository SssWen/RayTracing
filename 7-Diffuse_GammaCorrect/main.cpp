#include "Sphere.h"
#include "HitableList.h"
#include "Vec3.h"


#include <fstream>
#include <iostream>

using namespace std;

const Vec3f Sky(const Ray & ray);

const Vec3f Trace(Ptr<Hitable> scene, Ray & ray) {
	HitRecord rec;
	// 碰撞到，就再次沿着【碰撞点】法线方向随机进行漫反射，递归遍历，结束条件：没有碰撞，
	if (scene->Hit(ray, rec))
	{
		// 模拟漫反射，当前碰撞点，沿着法线方向周围，再次射出一条射线
		Vec3f dir = rec.n + Util::RandInSphere();
		Ray newRay(rec.p, dir.Normalize());
		return 0.5f * Trace(scene, newRay); // 每次反射一次，光照衰减一半
	}		
	return Sky(ray);// 未碰撞到，绘制天空背景
	// 最后的颜色为，0.5^反射次数*Sky(ray)
}

int main() {
	int width = 200;
	int height = 100;
	int sampleNum = 100;
	// 相机参数
	Vec3f pos(0.f);
	Vec3f lowerLeft(-2, -1, -1);
	Vec3f horizontal(4, 0, 0);
	Vec3f vertical(0, 2, 0);

	// 场景
	auto sphere = Sphere::New({ 0, 0, -1 }, 0.5f); // 中心点，半径
	auto ground = Sphere::New({ 0, -100.5, -1 }, 100.f);
	auto scene = HitableList::New({ sphere,ground });

	ofstream rst("../../Output/7-Diffuse+GammaCorrect.ppm"); // ppm 是一种简单的图片格式	

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右			
			Vec3f color(0);
			for (int k = 0; k < sampleNum; k++)
			{
				float u = float(i + Util::RandF()) / float(width);
				float v = float(height - j+ Util::RandF()) / float(height);
				Vec3f dir = lowerLeft + u * horizontal + v * vertical - pos;
				Ray ray(pos, dir);
				color += Trace(scene, ray);
			}			
			color /= float(sampleNum); // 求均值
			color = Util::Gamma(color); // Gamma矫正一次
			Vec3i iColor = 255.99f * color;
			rst << iColor.r << " " << iColor.g << " " << iColor.b << endl;
		}
	}

	rst.close();
	std::cout << "Output End" << std::endl;

	return 0;
}

const Vec3f Sky(const Ray & ray) {
	auto normDir = ray.d.Normalize();
	float t = 0.5f * (normDir.y + 1.0f);

	const Vec3f white(1.f);
	const Vec3f blue(0.5, 0.7, 1);

	return Vec3f::Lerp(white, blue, t); // 线性插值
}
