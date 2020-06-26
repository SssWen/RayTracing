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
#include <time.h>
using namespace std;

const Vec3f Sky(const Ray & ray);
const Vec3f Trace(Ptr<Hitable> scene, Ray & ray);
void SaveImg(const vector<vector<Vec3f>> & img);
Ptr<HitableList> GenScene();

int main() {
	int width = 200;
	int height = 100;
	int sampleNum = 10;
	int cpuNum = Util::NumCPU();
	// 相机参数
	Vec3f pos(13, 2, 3);
	Vec3f target(0, 0, 0);
	float focusDist = 10.0;
	float vfov = 20.f;
	float aspect = float(width) / float(height);
	float aperture = 0.1f;
	Camera camera(pos, target, vfov, aspect, aperture, focusDist);
	Ptr<HitableList> scene = GenScene();
	vector<vector<Vec3f>> img(height, vector<Vec3f>(width));
	vector<thread> workers;	
	vector<int> pixelNums(cpuNum, 0);
		
	clock_t start, end;
	start = clock();
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

	SaveImg(img);

	std::cout << "Output End" << std::endl;
	end = clock();
	printf("totle time = %f second\n", (float)(end - start)/ CLOCKS_PER_SEC);//秒
	return 0;
}
Ptr<HitableList> GenScene() {	
	auto balls = HitableList::New();
	auto ground = Sphere::New({ 0, -1000, 0 }, 1000.f, DiffuseMaterial::New(Vec3f{ 0.5f }));
	balls->push_back(ground);

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = Util::RandF();
			Vec3f center(a + 0.9f*Util::RandF(), 0.2f, b + 0.9f*Util::RandF());
			if ((center - Vec3f(4, 0.2, 0)).Norm() > 0.9f) {
				Ptr<Material> material;
				if (choose_mat < 0.8f) // diffuse				
					material = DiffuseMaterial::New({ Util::RandF()*Util::RandF(), Util::RandF()*Util::RandF(), Util::RandF()*Util::RandF() });									
				else if (choose_mat < 0.95f) // metal
					material = MetalMaterial::New({ 0.5f*(1 + Util::RandF()), 0.5f*(1.f + Util::RandF()), 0.5f*(1.f + Util::RandF()) }, 0.5f*Util::RandF());
				else // glass
					material = DieletricMaterial::New(1.5f);
				balls->push_back(Sphere::New(center, 0.2f, material));
			}
		}
	}
	balls->push_back(Sphere::New({ 0, 1, 0 }, 1.f, DieletricMaterial::New(1.5f)));
	balls->push_back(Sphere::New({ -4, 1, 0 }, 1.f, DiffuseMaterial::New({ 0.4, 0.2, 0.1 })));
	balls->push_back(Sphere::New({ 4, 1, 0 }, 1.f, MetalMaterial::New({ 0.7, 0.6, 0.5 }, 0.f)));

	return balls;
}
void SaveImg(const vector<vector<Vec3f>> & img) {
	int width = img.front().size();
	int height = img.size();
	
	ofstream rst("../../Output/13-Scene.ppm"); // ppm 是一种简单的图片格式	
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
