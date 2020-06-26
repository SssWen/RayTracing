#include "Sphere.h"
#include "HitableList.h"
#include "Vec3.h"
#include "DiffuseMaterial.h"
#include "MetalMaterial.h"
#include "DieletricMaterial.h"
#include "Camera.h"


#include <fstream>
#include <iostream>

using namespace std;

const Vec3f Sky(const Ray & ray);
const Vec3f Trace(Ptr<Hitable> scene, Ray & ray);

int main() {
	int width = 200;
	int height = 100;
	int sampleNum = 100;
	// 相机参数
	//Vec3f pos(0.f);
	//Vec3f lowerLeft(-2, -1, -1);
	//Vec3f horizontal(4, 0, 0);
	//Vec3f vertical(0, 2, 0);

	Vec3f pos(-2,2,1);
	Vec3f target(0,0,-1);
	float vfov = 25; // 张角
	float aspect = float(width) / float(height);
	Camera camera(pos, target, vfov, aspect);

	// 场景  中心点，半径,材质
	auto sphere = Sphere::New({ 0, 0, -1 }, 0.5f, DiffuseMaterial::New(Vec3f(0.8, 0.3, 0.3))); 
	auto ground = Sphere::New({ 0, -100.5, -1 }, 100.f, DiffuseMaterial::New(Vec3f(0.8, 0.8, 0)));
	// metal
	//auto sphereLeft = Sphere::New({ -1, 0, -1 }, 0.5f, MetalMaterial::New(Vec3f(0.8, 0.8, 0.8), 0.3));
	//auto sphereRight = Sphere::New({ 1, 0, -1 }, 0.5f, MetalMaterial::New(Vec3f(0.8, 0.6, 0.2), 1.0));
	// 电介质 折射
	auto sphereLeft = Sphere::New({ -1, 0, -1 }, 0.5f, DieletricMaterial::New(1.5f));		
	auto sphereLeftInner = Sphere::New({ -1, 0, -1 }, -0.45f, DieletricMaterial::New(1.5f));
	//半径为负的玻璃球放在上图玻璃球内部，就可以实现中空玻璃球的效果，	
	auto sphereRight = Sphere::New({ 1, 0, -1 }, 0.5f, MetalMaterial::New(Vec3f(0.8, 0.6, 0.2), 1.0));

	auto scene = HitableList::New({ sphere,sphereLeft,sphereLeftInner,sphereRight,ground });

	ofstream rst("../../Output/11-Camera.ppm"); // ppm 是一种简单的图片格式	

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右			
			Vec3f color(0);
			for (int k = 0; k < sampleNum; k++)
			{
				float u = float(i + Util::RandF()) / float(width);
				float v = float(height - j+ Util::RandF()) / float(height);
				//Vec3f dir = lowerLeft + u * horizontal + v * vertical - pos;
				//Ray ray(pos, dir);

				// 使用相机类获取Ray
				Ray ray = camera.GenRay(u, v);
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
const Vec3f Trace(Ptr<Hitable> scene, Ray & ray) {
	HitRecord rec;
	// 碰撞到，就再次沿着【碰撞点】法线方向随机进行漫反射，递归遍历，结束条件：没有碰撞，
	if (scene->Hit(ray, rec))
	{
		// 模拟漫反射，当前碰撞点，沿着法线方向周围，再次射出一条射线
		//Vec3f dir = rec.n + Util::RandInSphere();
		//Ray newRay(rec.p, dir.Normalize());
		//return 0.5f * Trace(scene, newRay); // 每次反射一次，光照衰减一半

		auto scatterRst = rec.material->Scatter(ray, rec); // 获取材质信息,反射方向,衰减参数
		if (!scatterRst.isScatter) // 反射角与法线相反,进入表面内部,表示光线被完全吸收
			return Vec3f(0);
		return scatterRst.attenuation * Trace(scene, scatterRst.ray);

	}
	return Sky(ray);// 未碰撞到，绘制天空背景
	// 最后的颜色为，scatterRst.attenuation^反射次数*Sky(ray)
}
const Vec3f Sky(const Ray & ray) {
	auto normDir = ray.d.Normalize();
	float t = 0.5f * (normDir.y + 1.0f);

	const Vec3f white(1.f);
	const Vec3f blue(0.5, 0.7, 1);

	return Vec3f::Lerp(white, blue, t); // 线性插值
}
