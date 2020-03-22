// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "matrix.h"

#pragma warning(disable: 4996)

int main() {
	double value1[] = { 1, 2, 3, 4 };
	double value2[] = { 2, 2, 2, 2 };
	double value3[] = { 3, 3, 3, 3 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt2(2, value2);
	NUDTTK::Matrix<double> mt3(2, value3);
	NUDTTK::Matrix<double> mt = (mt1 * mt2) + (mt3 * 6) / 2 - mt1;
	std::cout << mt.GetElement(0, 0);
	return 0;
}