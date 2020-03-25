// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#pragma warning(disable: 4996)

#include "matrix.h"
#include "math_algorithm.h"


int main() {
	Eigen::Matrix2d d1, d2;
	d1 << 1, 2, 3, 4;
	d2 << 2, 3, 4, 5;
	(d1 + d2).maxCoeff();
	return 0;
}