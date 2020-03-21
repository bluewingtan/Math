// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "matrix.h"

#pragma warning(disable: 4996)

int main() {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2, 2, 2, 2 };
	NUDTTK::Matrix<double> t1(2, value1), t2(2, value2);
	NUDTTK::Matrix<double> t = t1 * t2;
	std::cout << t.unwrap();
	return 0;
}