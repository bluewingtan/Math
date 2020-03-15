// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "matrix.h"

int main() {
	double value[] = { 3.0, 40.3251e222, 56556.3, -12.48 };
	NUDTTK::Matrix<double> mt(2, value);
	return 0;
}