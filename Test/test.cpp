#include "pch.h"

#include "../Math/matrix.h"

#pragma warning(disable: 4996)
TEST(matrix_initialization, default_constructor) {
	NUDTTK::Matrix<double> mt;

	EXPECT_EQ(mt.GetNumRows(), 0);
}

TEST(matrix_initialization, square) {
	NUDTTK::Matrix<double> mt(4);

	EXPECT_EQ(mt.GetNumRows(), 4);
	EXPECT_EQ(mt.GetNumColumns(), 4);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.0);
}

TEST(matrix_initialization, specified_size) {
	NUDTTK::Matrix<double> mt(4, 5);

	EXPECT_EQ(mt.GetNumRows(), 4);
	EXPECT_EQ(mt.GetNumColumns(), 5);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.0);
}

TEST(matrix_initialization, square_with_default_values) {
	double values[] = {4.0, 5.0, 6.0, 7.0};
	NUDTTK::Matrix<double> mt(2, values);

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 1), 5.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 0), 6.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 7.0);
}

TEST(matrix_initialization, specified_size_with_default_values) {
	double values[] = { 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
	NUDTTK::Matrix<double> mt(2, 3, values);

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 3);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 2), 6.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 8.0);
}

TEST(matrix_initialization, copy_constructor) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2(mt1);

	EXPECT_EQ(mt1.GetNumRows(), 2);
	EXPECT_EQ(mt1.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt1.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt1.GetElement(1, 0), 6.0);
	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt2.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, move_constructor) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2(std::move(mt1));

	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt2.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, copy_assignment) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2 = mt1;

	EXPECT_EQ(mt1.GetNumRows(), 2);
	EXPECT_EQ(mt1.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt1.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt1.GetElement(1, 0), 6.0);
	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt2.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, move_assignment) {
	double values[] = { 4.0, 5.0, 6.0, 7.0 };
	NUDTTK::Matrix<double> mt1(2, values);
	NUDTTK::Matrix<double> mt2 = std::move(mt1);

	EXPECT_EQ(mt2.GetNumRows(), 2);
	EXPECT_EQ(mt2.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt2.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt2.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, value_copy_constructor) {
	NUDTTK::Matrix<double>::base_type value(2, 2);
	value << 4.0, 5.0, 6.0, 7.0;
	NUDTTK::Matrix<double> mt(value);

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, value_copy_assignment) {
	NUDTTK::Matrix<double>::base_type value(2, 2);
	value << 4.0, 5.0, 6.0, 7.0;
	NUDTTK::Matrix<double> mt = value;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, value_move_constructor) {
	NUDTTK::Matrix<double>::base_type value(2, 2);
	value << 4.0, 5.0, 6.0, 7.0;
	NUDTTK::Matrix<double> mt(std::move(value));

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 0), 6.0);
}

TEST(matrix_initialization, value_move_assignment) {
	NUDTTK::Matrix<double>::base_type value(2, 2);
	value << 4.0, 5.0, 6.0, 7.0;
	NUDTTK::Matrix<double> mt = std::move(value);

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 0), 6.0);
}

TEST(matrix_operator, add_binary) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	NUDTTK::Matrix<double> mt1(2, value1), mt2(2, value2);
	NUDTTK::Matrix<double> mt = mt1 + mt2;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 2.1);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 2.4);
}

TEST(matrix_operator, add_binary_chain) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	double value3[] = { 0.6, 0.7, 0.8, 0.9 };
	NUDTTK::Matrix<double> mt1(2, value1), mt2(2, value2), mt3(2, value3);
	NUDTTK::Matrix<double> mt = mt1 + mt2 + mt3;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 2.7);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 3.3);
}

TEST(matrix_operator, sub_binary) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	NUDTTK::Matrix<double> mt1(2, value1), mt2(2, value2);
	NUDTTK::Matrix<double> mt = mt1 - mt2;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), -1.9);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), -1.6);
}

TEST(matrix_operator, sub_binary_chain) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	double value3[] = { 0.6, 0.7, 0.8, 0.9 };
	NUDTTK::Matrix<double> mt1(2, value1), mt2(2, value2), mt3(2, value3);
	NUDTTK::Matrix<double> mt = mt1 - mt2 - mt3;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), -2.5);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), -2.5);
}

TEST(matrix_operator, mul_binary_matrix) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	NUDTTK::Matrix<double> mt1(2, value1), mt2(2, value2);
	NUDTTK::Matrix<double> mt = mt1 * mt2;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.6);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 1.4);
}

TEST(matrix_operator, mul_binary_chain_matrix) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	double value3[] = { 0.6, 0.7, 0.8, 0.9 };
	NUDTTK::Matrix<double> mt1(2, value1), mt2(2, value2), mt3(2, value3);
	NUDTTK::Matrix<double> mt = mt1 * mt2 * mt3;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.84);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 2.24);
}

TEST(matrix_operator, mul_binary_scalar) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt = mt1 * 2.0;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.2);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 0.8);
}

TEST(matrix_operator, mul_binary_chain_scalar) {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt = 2.0 * mt1 * 2.0;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.4);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 1.6);
}

TEST(matrix_operator, div_binary_scalar) {
	double value1[] = { 2.0, 4.0, 6.0, 8.0 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt = mt1 / 2.0;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 1.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 4.0);
}

TEST(matrix_operator, div_binary_chain_scalar) {
	double value1[] = { 4.0, 8.0, 12.0, 16.0 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt = mt1 / 2.0 / 2.0;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 1.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 4.0);
}

TEST(matrix_operator, combination_operations) {
	double value1[] = { 1.0, 2.0, 3.0, 4.0 };
	double value2[] = { 2.0, 2.0, 2.0, 2.0 };
	double value3[] = { 3.0, 3.0, 3.0, 3.0 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt2(2, value2);
	NUDTTK::Matrix<double> mt3(2, value3);
	NUDTTK::Matrix<double> mt = (mt1 * mt2) + (mt3 * 6.0) / 2.0 - mt1;

	EXPECT_EQ(mt.GetNumRows(), 2);
	EXPECT_EQ(mt.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 8.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 13.0);
}

TEST(matrix_operator, compatibility_operations) {
	double value1[] = { 3.0, 3.0, 3.0, 3.0 };
	double value2[] = { 3.0, 3.0, 3.0, 3.0 };
	NUDTTK::Matrix<double> mt1(2, value1);
	NUDTTK::Matrix<double> mt2(2, value2);

	EXPECT_TRUE(mt1 == mt2);
	EXPECT_FALSE(mt1 != mt2);
}

TEST(matrix_operator, operator_rw_operations) {
	double value[] = { -2.0, 2.0, 2.0, -2.0 };
	NUDTTK::Matrix<double> mt(2, value);

	EXPECT_DOUBLE_EQ(mt(1, 1), -2.0);
	mt(1, 1) = 1.0;
	EXPECT_DOUBLE_EQ(mt(1, 1), 1.0);
}

TEST(matrix_function, absolute) {
	double value[] = { -2.0, 2.0, 2.0, -2.0 };
	NUDTTK::Matrix<double> mt(2, value);
	NUDTTK::Matrix<double> mt_abs(mt.Abs());
	
	EXPECT_EQ(mt_abs.GetNumRows(), 2);
	EXPECT_EQ(mt_abs.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt_abs.GetElement(0, 0), 2.0);
	EXPECT_DOUBLE_EQ(mt_abs.GetElement(1, 1), 2.0);
}

TEST(matrix_function, determinant) {
	double value[] = { -1.5, 2.0, 2.5, 3.0 };
	NUDTTK::Matrix<double> mt(2, value);

	EXPECT_DOUBLE_EQ(mt.DetGauss(), -9.5);
}

TEST(matrix_function, initialize) {
	NUDTTK::Matrix<double> mt;
	mt.Init(3, 3);
	
	EXPECT_EQ(mt.GetNumRows(), 3);
	EXPECT_EQ(mt.GetNumColumns(), 3);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 0.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 0.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(2, 2), 0.0);
}

TEST(matrix_function, make_unit) {
	NUDTTK::Matrix<double> mt;
	mt.MakeUnitMatrix(3);

	EXPECT_EQ(mt.GetNumRows(), 3);
	EXPECT_EQ(mt.GetNumColumns(), 3);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 0), 1.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 1), 1.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(2, 2), 1.0);
}

TEST(matrix_function, inverse) {
	double value[] = { -2.0, 1.0, 4.0, -3.0 };
	NUDTTK::Matrix<double> mt(2, value);
	NUDTTK::Matrix<double> mt_inv(mt.Inv());
	// Inv_Ssgj is an alias of Inv

	EXPECT_EQ(mt_inv.GetNumRows(), 2);
	EXPECT_EQ(mt_inv.GetNumColumns(), 2);
	EXPECT_DOUBLE_EQ(mt_inv.GetElement(0, 0), -1.5);
	EXPECT_DOUBLE_EQ(mt_inv.GetElement(1, 1), -1.0);
}

TEST(matrix_function, max) {
	double value[] = { -2.0, 1.0, 4.0, -3.0 };
	NUDTTK::Matrix<double> mt(2, value);
	size_t row_pos = 0, col_pos = 0;

	EXPECT_DOUBLE_EQ(mt.Max(), 4.0);
	EXPECT_DOUBLE_EQ(mt.Max(row_pos, col_pos), 4.0);
	EXPECT_EQ(row_pos, 1);
	EXPECT_EQ(col_pos, 0);
}

TEST(matrix_function, min) {
	double value[] = { -2.0, 1.0, 4.0, -3.0 };
	NUDTTK::Matrix<double> mt(2, value);
	size_t row_pos = 0, col_pos = 0;

	EXPECT_DOUBLE_EQ(mt.Min(), -3.0);
	EXPECT_DOUBLE_EQ(mt.Min(row_pos, col_pos), -3.0);
	EXPECT_EQ(row_pos, 1);
	EXPECT_EQ(col_pos, 1);
}

TEST(matrix_function, set_element) {
	double value[] = { -2.0, 1.0, 4.0, -3.0 };
	NUDTTK::Matrix<double> mt(2, value);

	EXPECT_DOUBLE_EQ(mt.GetElement(0, 1), 1.0);
	mt.SetElement(0, 1, 5.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(0, 1), 5.0);
}

TEST(matrix_function, transpose) {
	double value[] = { -2.0, 1.0, 4.0, -3.0 };
	NUDTTK::Matrix<double> mt(2, value);
	NUDTTK::Matrix<double> mt_t(mt.Transpose());

	EXPECT_DOUBLE_EQ(mt.GetElement(0, 1), 1.0);
	EXPECT_DOUBLE_EQ(mt_t.GetElement(0, 1), 4.0);
	EXPECT_DOUBLE_EQ(mt.GetElement(1, 0), 4.0);
	EXPECT_DOUBLE_EQ(mt_t.GetElement(1, 0), 1.0);
}