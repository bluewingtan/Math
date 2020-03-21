#include "pch.h"

#include "../Math/matrix.h"

TEST(matrix_initialization, default_constructor) {
	NUDTTK::Matrix<double> mt;

	EXPECT_EQ(mt.GetNumRows(), 0);
}

TEST(matrix_initialization, square) {
	NUDTTK::Matrix<double> mt(4);

	EXPECT_EQ(mt.GetNumRows(), 4);
	EXPECT_EQ(mt.GetNumColumns(), 4);
	EXPECT_EQ(mt.GetElement(0, 0), 0);
}

TEST(matrix_initialization, specified_size) {
	NUDTTK::Matrix<double> mt(4, 5);

	EXPECT_EQ(mt.GetNumRows(), 4);
	EXPECT_EQ(mt.GetNumColumns(), 5);
	EXPECT_EQ(mt.GetElement(0, 0), 0);
}

TEST(matrix_initialization, square_with_default_values) {
	double values[] = {4.0, 5.0, 6.0, 7.0};
	NUDTTK::Matrix<double> mt(2, values);

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_EQ(mt.GetElement(0, 0), 4.0);
	EXPECT_EQ(mt.GetElement(0, 1), 5.0);
	EXPECT_EQ(mt.GetElement(1, 0), 6.0);
	EXPECT_EQ(mt.GetElement(1, 1), 7.0);
}

TEST(matrix_initialization, specified_size_with_default_values) {
	double values[] = { 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
	NUDTTK::Matrix<double> mt(2, 3, values);

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 3);
	EXPECT_EQ(mt.GetElement(0, 2), 6.0);
	EXPECT_EQ(mt.GetElement(1, 1), 8.0);
}

TEST(matrix_initialization, copy_constructor) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2(mt1);

	EXPECT_EQ(mt1.GetNumRows(), 2);
	EXPECT_EQ(mt1.GetNumColumns(), 2);
	EXPECT_EQ(mt1.GetElement(0, 0), 4.0);
	EXPECT_EQ(mt1.GetElement(1, 0), 6.0);
	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_EQ(mt2.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, move_constructor) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2(std::move(mt1));

	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_EQ(mt2.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, copy_assignment) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2 = mt1;

	EXPECT_EQ(mt1.GetNumRows(), 2);
	EXPECT_EQ(mt1.GetNumColumns(), 2);
	EXPECT_EQ(mt1.GetElement(0, 0), 4.0);
	EXPECT_EQ(mt1.GetElement(1, 0), 6.0);
	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_EQ(mt2.GetElement(1, 0), 6.0);
}

