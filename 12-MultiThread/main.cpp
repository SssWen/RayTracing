#include "Sphere.h"
#include "HitableList.h"
#include "Vec3.h"
#include "DiffuseMaterial.h"
#include "MetalMaterial.h"
#include "DieletricMaterial.h"
#include "Camera.h"


#include <fstream>
#include <iostream>
#include <thread>
#include <array>

using namespace std;

const Vec3f Sky(const Ray & ray);
const Vec3f Trace(Ptr<Hitable> scene, Ray & ray);
void SaveImg(const vector<vector<Vec3f>> & img);
int main() {
	int width = 200;
	int height = 100;
	int sampleNum = 100;
	int cpuNum = Util::NumCPU();
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
	// 电介质 折射
	auto sphereLeft = Sphere::New({ -1, 0, -1 }, 0.5f, DieletricMaterial::New(1.5f));		
	auto sphereLeftInner = Sphere::New({ -1, 0, -1 }, -0.45f, DieletricMaterial::New(1.5f));
	//半径为负的玻璃球放在上图玻璃球内部，就可以实现中空玻璃球的效果，	
	auto sphereRight = Sphere::New({ 1, 0, -1 }, 0.5f, MetalMaterial::New(Vec3f(0.8, 0.6, 0.2), 1.0));

	auto scene = HitableList::New({ sphere,sphereLeft,sphereLeftInner,sphereRight,ground });

	vector<vector<Vec3f>> img(height, vector<Vec3f>(width));
	vector<thread> workers;
	vector<int> pixelNums(cpuNum, 0);

	ofstream rst("../../Output/11-Camera.ppm"); // ppm 是一种简单的图片格式	

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int id = 0; id < cpuNum; id++)
	{
		// 每个线程分配,i+nums*0,i+nums*1,i+nums*2... = width*height / cpuNums 个像素进行处理
		workers.push_back(thread([=, &img, &pixelNums](){
			for (int idx = id; idx < width*height; idx += cpuNum)
			{
				int y = idx / width;
				int x = idx - y * width;
				Vec3f color(0);
				for (int k=0;k<sampleNum;k++)
				{
					float u = (x + Util::RandF()) / width;
					float v = (height - y + Util::RandF()) / height; // 左下角
					Ray ray = camera.GenRay(u, v);
					color += Trace(scene, ray);
				}
				img[y][x] = color / (float)sampleNum;
				pixelNums[id]++;
				int sum = 0;
				for (auto pixelNum : pixelNums)
				{
					sum += pixelNum;
				}
				float rate = sum / float(width*height);
				
				printf("\r%.2f%% ...", rate*100.f); // printf是线程安全的，不能用cout
			}
		}));
	}
	for (auto & worker : workers) // 主线程等待所有子线程完成任务
		worker.join();

	rst.close();

	SaveImg(img);

	std::cout << "Output End" << std::endl;

	return 0;
}
void SaveImg(const vector<vector<Vec3f>> & img) {
	int width = img.front().size();
	int height = img.size();
	
	ofstream rst("../../Output/12-MultiThread.ppm"); // ppm 是一种简单的图片格式	
	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for (int i = 0; i < width; i++) { // 从左至右

			Vec3f gammaColor = Util::Gamma(img[j][i]);

			Vec3i iGammaColor = 255.99f * gammaColor;
			rst << iGammaColor.r << " " << iGammaColor.g << " " << iGammaColor.b << endl;
		}
	}

	rst.close();
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
