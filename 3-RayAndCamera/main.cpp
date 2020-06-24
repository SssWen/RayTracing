
#include <fstream>
#include <iostream>
#include "Vec3.h"
#include "Ray.h"

using namespace std;

const Vec3f Sky(const Ray & ray);

int main(){
	int width = 200;
	int height = 100;

	// 相机参数
	Vec3f camPos(0.f);
	Vec3f lowerLeft(-2, -1, -1);
	Vec3f horizontal(4, 0, 0);
	Vec3f vertical(0, 2, 0);

	ofstream rst("../../Output/03.ppm"); // ppm 是一种简单的图片格式	

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for(int i = 0; i < width; i++) { // 从左至右
			float u = float(i) / float(width);
			float v = float(height - j) / float(height);
			
			Vec3f dir = lowerLeft + u * horizontal + v * vertical - camPos;
			Ray ray(camPos, dir);

			auto skyColor = Sky(ray);
			Vec3i iSkyColor = 255.99f*skyColor;
			rst << iSkyColor.r << " " << iSkyColor.g << " " << iSkyColor.b << endl;
		}
	}

	rst.close();
	std::cout << "Output End" << std::endl;	
	return 0;
	
}

// 模拟天空颜色
const Vec3f Sky(const Ray& ray) {
	auto normDir = ray.d.Normalize();
	float t = 0.5f * (normDir.y + 1.0f); // 将法向的范围映射到 [0, 1] 以可视化
	const Vec3f white(1.f);
	const Vec3f blue(0.5, 0.7, 1);

	return Vec3f::Lerp(white, blue, t); // 线性插值
}