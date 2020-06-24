
#include <fstream>
#include <iostream>
#include "Vec3.h"
using namespace std;

int main(){
	int width = 200;
	int height = 100;

	ofstream rst("../../Output/02.ppm"); // ppm 是一种简单的图片格式	

	rst << "P3\n" << width << " " << height << "\n255\n";

	for (int j = 0; j < height; j++) { // 从上至下
		for(int i = 0; i < width; i++) { // 从左至右
			float r = float(i) / float(width);
			float g = float(height - j) / float(height);
			float b = 0.2f;

			// 浮点数颜色 [0, 1] 转化成整数颜色 [0, 255]
			Vec3f color(r, g, b);
			Vec3i icolor = 255.99f*color;
			rst << icolor.r << " " << icolor.g << " " << icolor.b << endl;
		}
	}

	rst.close();
	std::cout << "Output End" << std::endl;	
	return 0;
	
}