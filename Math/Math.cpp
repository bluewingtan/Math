// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "matrix.h"

#pragma warning(disable: 4996)

int main() {
	NUDTTK::Matrix mt(3, 3);
	std::cout << mt.ToString();
	return 0;
}