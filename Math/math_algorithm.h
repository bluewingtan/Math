#pragma once

#ifndef _NUDTTK_MATH_ALGORITHM_TR_
#define _NUDTTK_MATH_ALGORITHM_TR_

#include "common.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>

#include "matrix.h"

#if __cplusplus >= 201103L
#include <memory>
#include <numeric>
#include <random>
#else	// __cplusplus < 201103L
#include <boost/scoped_array.hpp>
#include <boost/algorithm/cxx11/iota.hpp>
#include <boost/random.hpp>
#endif	// __cplusplus >= 201103L


// Performance tips
#ifdef _MSC_VER				// MSVC
#if _MSC_VER < 1900
// Visual Studio 2015 below
#pragma message("Please use Visual Studio 2015 and above to support constexpr feature for better performance.")
#endif	// _MSC_VER < 1900
#elif defined __GNUC__		// GCC
#if __GNUC__ < 5
＃warning("Please use GCC 5 and above to support constexpr feature for better performance.")
#endif	// __GNUC__ < 5
#elif defined __clang__		// llvm
#if __clang_major__ < 3 && __clang_minor__ < 4
＃warning("Please use clang 3.4 and above to support constexpr feature for better performance.")
#endif	// __clang_major__ < 3 && __clang_minor__ < 4
#elif __cplusplus < 201402L	// Other compiler
#pragma message("Please use C++ 14 and above to support constexpr feature for better performance.")
#endif	// #ifdef _MSC_VER

namespace NUDTTK {
	namespace Math {
		/// <summary> Rounds the given number. </summary>
		/// <remarks> Blue Wing, 2020/3/8. </remarks>
		/// <param name="x"> The number to be rounded. </param>
		/// <returns> The rounded number. </returns>
		double Round(const double x) _NOEXCEPT {
			return std::round(x);
		}

		/// <summary>
		///		<para> Calculate factorials of the given number. </para>
		///		<para> Because of the limitation of type-double, this function may only support <c>n</c> below 25. </para>
		///	</summary>
		/// <remarks>
		/// 	<para> Blue Wing, 2020/3/8. </para>
		///		<para>
		///			If we store the result of odd multiplication and power of 2, then we can reduce the time complexity of the algorithm to O(1).
		///			So we could reduce the number of multiplications by log2(n) times.
		///		</para>
		///		<para> If we can use a modern C ++ compiler, we can enjoy the performance optimizations brought by constexpr, 
		///			   whether it is C ++ 11 or later.
		///		</para>
		/// </remarks>
		/// <param name="n"> A number to process. </param>
		/// <returns> Factorials. </returns>
#if __cplusplus == 201103L
		_CONSTEXPR_FN_A double factorial(const size_t n) _NOEXCEPT {
			return n == 0 ? 1 : n * factorial(n - 1);
		}
#else	// __cplusplus != 201103L
		_CONSTEXPR_FN double Factorial(const size_t n) _NOEXCEPT {
			// The odd number multiplication table
			// Store PI(i=1,n){ 2 * i - 1 } from n = 1
			_CONSTEXPR double odd_multiplication_table[32] = {
				1.0                    , 3.0                    , 15.0                   , 105.0                  ,
				945.0                  , 10395.0                , 135135.0               , 2027025.0              ,
				34459425.0             , 654729075.0            , 13749310575.0          , 316234143225.0         ,
				7905853580625.0        , 213458046676875.0      , 6190283353629375.0     , 1.9189878396251062e+17 ,
				6.33265987076285e+18   , 2.2164309547669976e+20 , 8.200794532637891e+21  , 3.1983098677287775e+23 ,
				1.3113070457687988e+25 , 5.6386202968058344e+26 , 2.5373791335626256e+28 , 1.192568192774434e+30  ,
				5.843584144594727e+31  , 2.980227913743311e+33  , 1.5795207942839547e+35 , 8.687364368561751e+36  ,
				4.951797690080198e+38  , 2.9215606371473166e+40 , 1.782151988659863e+42  , 1.1227557528557138e+44
			};

			// The pow2 number table
			// Store 2^n from n = 0
			_CONSTEXPR double pow2_table[32] = {
				1.0                    , 2.0                    , 4.0                    , 8.0                    ,
				16.0                   , 32.0                   , 64.0                   , 128.0                  ,
				256.0                  , 512.0                  , 1024.0                 , 2048.0                 ,
				4096.0                 , 8192.0                 , 16384.0                , 32768.0                ,
				65536.0                , 131072.0               , 262144.0               , 524288.0               ,
				1048576.0              , 2097152.0              , 4194304.0              , 8388608.0              ,
				16777216.0             , 33554432.0             , 67108864.0             , 134217728.0            ,
				268435456.0            , 536870912.0            , 1073741824.0           , 2147483648.0
			};

			size_t shift_idx = n;					// Store powers of 2 in each round of calculation
			size_t shift_num = 0;					// The final power of 2
			size_t odd_table_idx = (n + 1) >> 1;	// Store odd multiplication table index in each round of calculation
			double factorial_num = 1.0;				// Final result

			// Using shift instead of multiplication for better performance
			for (; shift_idx != 1; odd_table_idx >>= 1) {
				factorial_num *= odd_multiplication_table[odd_table_idx - 1];
				odd_table_idx = shift_idx - odd_table_idx + 1;
				shift_idx >>= 1;
				shift_num += shift_idx;
			}

			factorial_num *= pow2_table[shift_num];

			return factorial_num;
		}
#endif	// __cplusplus != 201103L

		/// <summary> Sort number list with given size and order. </summary>
		/// <remarks> Blue Wing, 2020/3/8. </remarks>
		/// <typeparam name="_T"> Numerical type </typeparam>
		/// <param name="sort_list"> The number list to be sorted. </param>
		/// <param name="list_size"> Size of x list. </param>
		/// <param name="ascend">    True to ascend. </param>
		template<typename _T>
		_CONSTEXPR_FN void Sort(_T sort_list[], const size_t list_size, const bool ascend = true) _NOEXCEPT {
			if (ascend) {
				// ASC
				std::sort(sort_list, sort_list + list_size, std::less<_T>());
			} else {
				// DESC
				std::sort(sort_list, sort_list + list_size, std::greater<_T>());
			}
		}

		/// <summary> 
		/// 	<para> Sort number list with given size and order. </para>
		///		<para> Ensure that the same values ​​isotonic order in list. </para>
		///	</summary>
		/// <remarks> Blue Wing, 2020/3/8. </remarks>
		/// <typeparam name="_T"> Numerical type </typeparam>
		/// <param name="sort_list">	   The number list to be sorted. </param>
		/// <param name="sort_index_list"> The sequence of number list(should remain uninitialized). </param>
		/// <param name="list_size">	   Size of x list. </param>
		/// <param name="ascend">		   True to ascend. </param>
		template<typename _T>
		_CONSTEXPR_FN void Sort(_T sort_list[], size_t sort_index_list[], const size_t list_size, const bool ascend = true) _NOEXCEPT {
			// Initialize original index locations
#if __cplusplus >= 201103L
			std::iota(sort_index_list, sort_index_list + list_size, 0);
#else
			boost::algorithm::itoa(sort_index_list, sort_index_list + list_size, 0);
#endif

			if (ascend) {
				// ASC
#if __cplusplus >= 201103L
				std::stable_sort(sort_index_list, sort_index_list + list_size,
								 [&sort_list](size_t i1, size_t i2) {return sort_list[i1] < sort_list[i2]; });
#else
				// Callable functor
				struct {
					_T* sort_list;
					bool operator()(size_t i1, size_t i2) const {
						return sort_list[i1] < sort_list[i2];
					}
				} Less;
				Less.sort_list = sort_list;
				std::stable_sort(sort_index_list, sort_index_list + list_size, Less);
#endif	// __cplusplus >= 201103L
				std::sort(sort_list, sort_list + list_size, std::less<_T>());
			} else {
				// DESC
#if __cplusplus >= 201103L
				std::stable_sort(sort_index_list, sort_index_list + list_size,
								 [&sort_list](size_t i1, size_t i2) {return sort_list[i1] > sort_list[i2]; });
#else
				// Callable functor
				struct {
					_T* sort_list;
					bool operator()(size_t i1, size_t i2) const {
						return sort_list[i1] > sort_list[i2];
					}
				} Greater;
				Greater.sort_list = sort_list;
				std::stable_sort(sort_index_list, sort_index_list + list_size, Greater);
#endif	// __cplusplus >= 201103L
				std::sort(sort_list, sort_list + list_size, std::greater<_T>());
			}
		}

		/// <summary> Calculate normalized Legendre function values using recursion. </summary>
		/// <remarks>
		/// 	<para> Gu Defeng, 2007/11/23. </para>
		///		<para> Blue Wing, 2020/3/8. </para>
		/// </remarks>
		/// <param name="P"> [in,out] If non-null, Legendre function value [n + 1][n + 1]. </param>
		/// <param name="n"> Order, n>=2. </param>
		/// <param name="u"> sin(fai), [-1,1]. </param>
		void LegendreFunc(long double** P, const size_t n, const double u) _NOEXCEPT {
			const double cosu = std::sqrt(1 - u * u);
			for (size_t i = 0; i <= n; i++) {
				memset(P[i], 0, sizeof(long double) * (n + 1));
			}
			// Calculate the harmonic term
			P[0][0] = 1;
			P[1][0] = std::sqrt(3.0) * u;
			for (size_t i = 2; i <= n; i++)
				P[i][0] = (std::sqrt(4.0 * i * i - 1.0) / i) * u * P[i - 1][0]
				- ((i - 1.0) / i) * std::sqrt((2.0 * i + 1.0) / (2.0 * i - 3.0)) * P[i - 2][0];
			// Calculate fan harmonics, diagonal elements
			P[1][1] = std::sqrt(3.0) * cosu;
			for (size_t i = 2; i <= n; i++)
				P[i][i] = std::sqrt((2.0 * i + 1.0) / (2.0 * i)) * cosu * P[i - 1][i - 1];
			for (size_t i = 2; i <= n; i++)
				P[i][i - 1] = std::sqrt(2.0 * i + 1.0) * u * P[i - 1][i - 1];
			// Calculate the field harmonic term
			for (size_t i = 3; i <= n; i++) {
				for (size_t j = 1; j <= i - 2; j++) {
					P[i][j] = std::sqrt((4.0 * i * i - 1.0) / (static_cast<double>(i) * i - static_cast<double>(j) * j)) * u * P[i - 1][j]
						- std::sqrt(((2.0 * i + 1.0) * (i - 1.0 + j) * (i - 1.0 - j)) / ((2.0 * i - 3.0) * (static_cast<double>(i) + j) * (i - j))) * P[i - 2][j];
				}
			}
		}

		/// <summary> Calculate normalized Legendre function values using recursion. </summary>
		/// <remarks>
		/// 	<para> Gu Defeng, 2007/11/23. </para>
		///		<para> Blue Wing, 2020/3/8. </para>
		/// </remarks>
		/// <param name="P">  [in,out] If non-null, Legendre function value [n + 1][n + 1]. </param>
		/// <param name="DP"> [in,out] If non-null, derivative value of Legendre function [n + 1][n + 1]. </param>
		/// <param name="n">  Order, n>=2. </param>
		/// <param name="u">  sin(fai), [-1,1]. </param>
		void LegendreFuncDerivative(long double** P, long double** DP, const size_t n, const double u) _NOEXCEPT {
			// First calculate the Legendre function value P
			LegendreFunc(P, n, u);
			// Recursively calculate the derivative value of Legendre function DP
			double cosu = std::sqrt(1.0 - u * u);
			for (size_t i = 0; i <= n; i++) {
				memset(DP[i], 0, sizeof(long double) * (n + 1));
			}
			// Calculate the harmonic term
			DP[0][0] = 0;
			DP[1][0] = std::sqrt(3.0) * cosu;
			for (size_t i = 2; i <= n; i++)
				DP[i][0] = (std::sqrt(4.0 * i * i - 1.0) / i) * (u * DP[i - 1][0] + cosu * P[i - 1][0])
				- ((i - 1.0) / i) * std::sqrt((2.0 * i + 1.0) / (2.0 * i - 3.0)) * DP[i - 2][0];
			// Calculate fan harmonics, diagonal elements
			DP[1][1] = -std::sqrt(3.0) * u;
			for (size_t i = 2; i <= n; i++)
				DP[i][i] = std::sqrt((2.0 * i + 1.0) / (2.0 * i)) * (-u * P[i - 1][i - 1] + cosu * DP[i - 1][i - 1]);
			// Calculate the field harmonic term
			for (size_t i = 2; i <= n; i++)
				DP[i][i - 1] = std::sqrt(2.0 * i + 1.0) * (u * DP[i - 1][i - 1] + cosu * P[i - 1][i - 1]);
			for (size_t i = 3; i <= n; i++)
				for (size_t j = 1; j <= i - 2; j++)
					DP[i][j] = std::sqrt((4.0 * i * i - 1.0) / (static_cast<double>(i) * i - static_cast<double>(j) * j)) * (u * DP[i - 1][j] + cosu * P[i - 1][j])
					- std::sqrt(((2.0 * i + 1.0) * (i - 1.0 + j) * (i - 1.0 - j)) / ((2.0 * i - 3.0) * (static_cast<double>(i) + j) * (static_cast<double>(i) - j))) * DP[i - 2][j];
		}

		/// <summary> Calculate the number of combinations. </summary>
		/// <remarks> 
		/// 	<para> Blue Wing, 2020/3/8. </para>
		///		<para>
		///			If we can use a modern C ++ compiler, we can enjoy the performance optimizations brought by constexpr,
		///			whether it is C ++ 11 or later.
		///		</para>
		/// </remarks>
		/// <param name="n"> Total element number. </param>
		/// <param name="m"> Take out the number of element. </param>
		/// <returns> Number of combinations. </returns>
#if __cplusplus >= 201103L
		_CONSTEXPR_FN_A double Combination(const size_t n, const size_t m) _NOEXCEPT
#else
		double Combination(const size_t n, const size_t m)
#endif
		{
			// [NOTE]
			// Actually, we should use Dynamic Programming (DP) to solve this for better performance,
			// since our Factorial function is fast enough, we don't need this
			return Factorial(n) / (Factorial(n - m) * Factorial(m));
		}

		/// <summary> Interploation lagrange. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/1/23. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="xa"> Known point abscissa. </param>
		/// <param name="ya"> Known point ordinate. </param>
		/// <param name="n">  Number of known points, order n-1. </param>
		/// <param name="x">  Interpolation point abscissa. </param>
		/// <param name="y">  [in,out] Interpolation point ordinate. </param>
		/// <param name="dy"> [in,out] Interpolation point speed. </param>
		_CONSTEXPR_FN_A void InterploationLagrange(double xa[], double ya[], const size_t n, const double x, double& y, double& dy) _NOEXCEPT {
			// Determine if the interpolation point falls on a known point
			int nFlag = -1;
			for (size_t k = 0; k < n; k++) {
				if (std::fabs(x - xa[k]) < 1.0E-16) {
					nFlag = k;		// The point to be interpolated falls exactly on the k-th reference point
					break;
				}
			}
			y = 0; dy = 0;
			// If the interpolation point does not fall on a known point
			// the derivative calculation process can be simplified at this time
			if (nFlag == -1) {
				for (size_t k = 0; k < n; k++) {
					double L = 1;	// K-th interpolation polynomial
					for (size_t j = 0; j < n; j++) {
						if (k != j)
							L = L * (x - xa[j]) / (xa[k] - xa[j]);
					}
					y = y + ya[k] * L;
					double dL = 0;
					for (size_t j = 0; j < n; j++) {
						if (k != j)
							dL = dL + L / (x - xa[j]);	// Deduct a multiplier for each term
					}
					dy = dy + ya[k] * dL;
				}
			} else {
				// The interpolation point falls exactly on a known point
				for (size_t k = 0; k < n; k++) {
					double L = 1.0, denominator = 1.0;
					for (size_t j = 0; j < n; j++) {
						if (k != j) {
							L = L * (x - xa[j]) / (xa[k] - xa[j]);
							denominator = denominator * (xa[k] - xa[j]);
						}
					}
					// Cumulative y
					y = y + ya[k] * L;
					if (nFlag == k)	// The interpolation point falls exactly on the k-th known point
					{
						double dL = 0;
						for (size_t j = 0; j < n; j++) {
							if (k != j) {
								double numerator = 1;
								for (size_t i = 0; i < n; i++) {
									if (k != i && j != i)
										numerator = numerator * (x - xa[i]);	// Molecular multiplication
								}
								dL = dL + numerator / denominator;				// Accumulation of partial derivatives
							}
						}
						dy = dy + ya[k] * dL;
					} else {
						// The interpolation point does not fall on the k-th known point
						// only one item is non-zero at this time
						double numerator = 1;
						for (size_t j = 0; j < n; j++) {
							if (k != j && nFlag != j)
								numerator = numerator * (x - xa[j]);			// Molecular multiplication
						}
						double dL = numerator / denominator;
						dy = dy + ya[k] * dL;
					}
				}
			}
		}

		/// <summary> Interploation lagrange. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/1/23. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="xa"> Known point abscissa. </param>
		/// <param name="ya"> Known point ordinate. </param>
		/// <param name="n">  Number of known points, order n-1. </param>
		/// <param name="x">  Interpolation point abscissa. </param>
		/// <param name="y">  [in,out] Interpolation point ordinate. </param>
		_CONSTEXPR_FN_A void InterploationLagrange(double xa[], double ya[], const size_t n, const double x, double& y) _NOEXCEPT {
			double dy = 0;
			InterploationLagrange(xa, ya, n, x, y, dy);
		}

		/// <summary>
		/// 	<para> Determines the median of the given parameters. </para>
		///		<para> Will <b>CHANGE</b> element in the element list. </para>
		/// </summary>
		/// <remarks> Blue Wing, 2020/3/9. </remarks>
		/// <typeparam name="_T"> Element type. </typeparam>
		/// <param name="element_list"> List of elements. </param>
		/// <param name="element_num">  Number of elements. </param>
		/// <exception> If <c>list_size</c> smaller than 2, will lead to undefined behavior. </exception>
		/// <returns> The median value. </returns>
		template<typename _T>
		_CONSTEXPR_FN _T Median(_T element_list[], const size_t list_size) _NOEXCEPT {
			if (list_size & 1) {
				// Odd number [O(n)]
				std::nth_element(element_list, element_list + list_size / 2, element_list + list_size);
				return element_list[list_size / 2];
			} else {
				// Even number [O(nlog(n))]
				std::sort(element_list, element_list + list_size);
				return element_list[list_size / 2] + element_list[list_size / 2 - 1];
			}
		}

		/// <summary> 
		/// 	<para> Median absolute deviation. </para>
		///		<para> Actually this function should be named scaled Median Absolute Deviation (sMAD) </para>
		///		<para> according to https://www.itl.nist.gov/div898/software/dataplot/refman2/auxillar/mad.htm </para>
		/// </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/5/11. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <typeparam name="_T"> Type of the t. </typeparam>
		/// <param name="observation_list"> List of observations. </param>
		/// <param name="list_size">	    Size of the list. </param>
		/// <returns> The sMAD value. </returns>
		template<typename _T>
		_CONSTEXPR_FN double Mad(const _T observation_list[], const size_t list_size) _NOEXCEPT {
			std::vector<_T> list(observation_list, observation_list + list_size);
#if __cplusplus >= 201103L
			std::for_each(list.begin(), list.end(), [&](_T& value) {value = std::abs(value); });
#else	// __cplusplus < 201103L
			struct {
				void operator()(_T& value) { value = std::abs(value); }
			} abs_functor;
			std::for_each(list.begin(), list.end(), abs_functor());
#endif	// __cplusplus >= 201103L
			return static_cast<double>(Median(list.data(), list_size)) / 0.6745;
		}

		/// <summary> Polygon fit. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/11/18. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="x">	 The range of the abscissa x [i] is [-1,1]. </param>
		/// <param name="y">	 The y coordinate. </param>
		/// <param name="n">	 Number of data to be smoothed. </param>
		/// <param name="y_fit"> Smoothed output value. </param>
		/// <param name="m">	 (Optional) Polynomial order, 2 = <m <= n, default is 3. </param>
		/// <returns> True if it succeeds, false if it fails. </returns>
		bool PolyFit(double x[], double y[], const size_t n, double y_fit[], const size_t m = 3) _NOEXCEPT {
			if (m < 1 || m > n)
				return false;

			Matrix matC(n, m);
			Matrix matY(n, 1);
			std::vector<double> base(n, 0);

			for (size_t i = 0; i < n; i++)
				base[i] = x[i] - x[0];

			for (size_t i = 0; i < n; i++) {
				matY(i, 0) = y[i];
				matC(i, 0) = 1.0;
				for (size_t j = 1; j < m; j++)
					matC(i, j) = std::pow(base[i], j);
			}
			Matrix matS = Matrix(matC.Transpose() * matC).Inv() * matC.Transpose() * matY;
			Matrix matY_Fit = matC * matS;
			for (size_t i = 0; i < n; i++)
				y_fit[i] = matY_Fit(i, 0);

			return true;
		}

		/// <summary> Vandrak filter. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/1/21, 2012/4/13. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="x">	 Observed data abscissa. </param>
		/// <param name="y">	 Observed data ordinate. </param>
		/// <param name="w">	 Observation weight, default equal weight. </param>
		/// <param name="n">	 Number of observations. </param>
		/// <param name="eps_v"> Smoothing factor. </param>
		/// <param name="y_fit"> [Out] Output value of vandrak smooth fit. </param>
		/// <returns> True if it succeeds, false if it fails. </returns>
		bool VandrakFilter(double x[], double y[], double w[],
						   const size_t n, const double eps_v, double y_fit[]) _NOEXCEPT {
			// Vandrak fitting requires at least 4 data
			if (n < 4)
				return false;
			double eps = eps_v * (x[n - 2] - x[1]) / (n - 3);
			Matrix matABCD(4, n - 3);
			for (size_t i = 0; i < n - 3; i++) {
				double a = 6 * std::sqrt(x[i + 2] - x[i + 1]) / ((x[i] - x[i + 1]) * (x[i] - x[i + 2]) * (x[i] - x[i + 3]));
				double b = 6 * std::sqrt(x[i + 2] - x[i + 1]) / ((x[i + 1] - x[i]) * (x[i + 1] - x[i + 2]) * (x[i + 1] - x[i + 3]));
				double c = 6 * std::sqrt(x[i + 2] - x[i + 1]) / ((x[i + 2] - x[i]) * (x[i + 2] - x[i + 1]) * (x[i + 2] - x[i + 3]));
				double d = 6 * std::sqrt(x[i + 2] - x[i + 1]) / ((x[i + 3] - x[i]) * (x[i + 3] - x[i + 1]) * (x[i + 3] - x[i + 2]));
				matABCD(0, i) = a;
				matABCD(1, i) = b;
				matABCD(2, i) = c;
				matABCD(3, i) = d;
			}
			Matrix matY(n, 1);
			for (size_t i = 0; i < n; i++)
				matY(i, 0) = y[i] * w[i] * eps; /*Y = (X(2,:).*P)'.*e;*/

			Matrix matA(7, n);
			/* Seven-diagonal matrix, using a 7-row structure to record
			   the optimized matrix row number = 3-(i-j), while keeping the column structure unchanged
			   matA(0,:)------ i - j =  3, i = 3:n-1, j = 0:n-4， n-3 elements
			   matA(1,:)------ i - j =  2, i = 2:n-1, j = 0:n-3， n-2 elements
			   matA(2,:)------ i - j =  1, i = 1:n-1, j = 0:n-2， n-1 elements
			   matA(3,:)------ i - j =  0, i = 0:n-1, j = 0:n-1， n   elements
			   matA(4,:)------ i - j = -1, i = 0:n-2, j = 1:n-1， n-1 elements
			   matA(5,:)------ i - j = -2, i = 0:n-3, j = 2:n-1， n-2 elements
			   matA(6,:)------ i - j = -3, i = 0:n-4, j = 3:n-1， n-3 elements

			  |3  4  5  6  *  *  *|
			  |2  3  4  5  6  *  *|
			  |1  2  3  4  5  6  *|
			  |0  1  2  3  4  5  6|
			  |*  0  1  2  3  4  5|
			  |*  *  0  1  2  3  4|
			  |*  *  *  0  1  2  3|
			*/

			for (size_t j = 0; j < n; j++) {
				matA(3, j) = w[j] * eps;
				for (size_t i = j; i >= j - 3; i--) {
					int nCount = j - i;
					if (i >= 0 && i <= n - 4) {
						// Make sure [0, n-4]
						matA(3, j ) = matA(3, j) + std::pow(matABCD(nCount, i), 2);
					}
				}
				// A_1j--> matA(4,:)
				for (size_t i = j; i >= j - 2; i--) {
					int nCount = j - i;
					if (i >= 0 && i <= n - 4) {
						// Make sure [0, n-4]
						matA(4, j + 1) = matA(4, j + 1) + matABCD(nCount, i) * matABCD(nCount + 1, i);
					}
				}
				// A_2j--> matA(5,:)
				for (size_t i = j; i >= j - 1; i--) {
					int nCount = j - i;
					if (i >= 0 && i <= n - 4) {
						// Make sure [0, n-4]
						matA(5, j + 2) = matA(5, j + 2) + matABCD(nCount, i) * matABCD(nCount + 2, i);
					}
				}
				// A_3j--> matA(6,:)
				if (j <= n - 4) {
					matA(6, j + 3) = matABCD(0, j) * matABCD(3, j);
				}
				// A_-1j--> matA(2,:)
				for (size_t i = j - 1; i >= j - 3; i--) {
					int nCount = j - 1 - i;
					if (i >= 0 && i <= n - 4) {
						// Make sure [0, n-4]
						matA(2, j - 1) = matA(2, j - 1) + matABCD(nCount, i) * matABCD(nCount + 1, i);
					}
				}
				// A_-2j--> matA(1,:)
				for (size_t i = j - 2; i >= j - 3; i--) {
					int nCount = j - 2 - i;
					if (i >= 0 && i <= n - 4) {
						// Make sure [0, n-4]
						matA(1, j - 2) = matA(1, j - 2) + matABCD(nCount, i) * matABCD(nCount + 2, i);
					}
				}
				// A_-3j--> matA(0,:)
				if (j - 3 >= 0 && j - 3 <= n - 4) {
					//matA.SetElement(j,j-3,matABCD(0,j-3)*matABCD(3,j-3));
					matA(0, j - 3) = matABCD(0, j - 3) * matABCD(3, j - 3);
				}
			}

			// 20071013, changed the solution method of Vondrak fitting to the catch-up method
			// Chasing method for solving 7-diagonal linear equations
			for (size_t i = 0; i < n - 1; i++) {
				// Unitize by diagonal elements
				double element_ii = matA(3, i);
				matY(i, 0) = matY(i, 0) / element_ii;
				for (size_t k = i; k < n; k++) {
					if (3 - (i - k) <= 6) {
						matA(3 - (i - k), k) = matA(3 - (i - k), k) / element_ii;
					}
				}

				size_t N = (i + 3 < n - 1) ? i + 3 : n - 1;
				for (size_t j = i + 1; j <= N; j++) {
					// Elimination is performed line by line, with a maximum of 3 lines
					double element_ji = matA(3 - (j - i), i);
					matY(j, 0) = matY(j, 0) - element_ji * matY(i, 0);
					for (size_t k = i; k < n; k++) {
						if (3 - (i - k) <= 6) {
							matA(3 - (j - k), k) = matA(3 - (j - k), k) - element_ji * matA(3 - (i - k), k);
						}
					}
				}
			}
			// The last line
			matY(n - 1, 0) = matY(n - 1, 0) / matA(3, n - 1);
			matA(3, n - 1) = 1;
			// Chase solution
			y_fit[n - 1] = matY(n - 1, 0);
			y_fit[n - 2] = matY(n - 2, 0) - matA(4, n - 1) * y_fit[n - 1];
			y_fit[n - 3] = matY(n - 3, 0) - matA(4, n - 2) * y_fit[n - 2] - matA(5, n - 1) * y_fit[n - 1];
			y_fit[n - 4] = matY(n - 4, 0) - matA(4, n - 3) * y_fit[n - 3] - matA(5, n - 2) * y_fit[n - 2] - matA(6, n - 1) * y_fit[n - 1];
			for (int i = n - 5; i >= 0; i--) {
				y_fit[i] = matY(i, 0) - matA(4, i + 1) * y_fit[i + 1] - matA(5, i + 2) * y_fit[i + 2] - matA(6, i + 3) * y_fit[i + 3];
			}
			return true;
		}

		/// <summary>
		///		Kinematic robust vandrak filter, 
		///		suitable for the occasion of observing the dynamic change of noise.
		///	</summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/5/11. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="x">			 Observed data abscissa. </param>
		/// <param name="y">			 Observed data ordinate. </param>
		/// <param name="w">			 Observation weight 0 or 1, w can contain outlier information. </param>
		/// <param name="n">			 Number of observations. </param>
		/// <param name="eps">			 Smoothing factor. </param>
		/// <param name="y_fit">		 Output value of Vandrak smooth fit. </param>
		/// <param name="threshold_max"> Maximum threshold of standard deviation. </param>
		/// <param name="threshold_min"> Minimum threshold of standard deviation. </param>
		/// <param name="nwidth">		 Window width of dynamic noise. </param>
		/// <param name="factor">		 (Optional) Robust factor, default is 3. </param>
		/// <returns> True if it succeeds, false if it fails. </returns>
		bool KinematicRobustVandrakFilter(double x[], double y[], double w[],
										  const size_t n, double eps, double y_fit[],
										  const double threshold_max, const double threshold_min,
										  const size_t nwidth, const double factor = 3) _NOEXCEPT {
			bool bResult = true;
#if __cplusplus >= 201103L
			std::unique_ptr<double[]> error_fit(new double[n]());
			std::unique_ptr<double[]> pmad(new double[n]());
			std::unique_ptr<double[]> w_old(new double[n]());
			std::unique_ptr<double[]> w_new(new double[n]());
#else	//__cplusplus < 201103L
			boost::scoped_array<double> error_fit(new double[n]());
			boost::scoped_array<double> pmad(new double[n]());
			boost::scoped_array<double> w_old(new double[n]());
			boost::scoped_array<double> w_new(new double[n]());
#endif  // __cplusplus >= 201103L
			std::copy(w, w + n, w_old.get());			// w -- contains the original wild value mark
			size_t nLoop = 0;
			_CONSTEXPR size_t nLoop_max = 6;		// Set a threshold for the number of iterations
													// to prevent iterations from oscillating at the critical point and fail to converge
			int n_valid = 0;
			while (true) {
				// Perform a Vandrak fit
				if (!VandrakFilter(x, y, w_old.get(), n, eps, y_fit)) {
					bResult = false;
					break;
				}
				nLoop++;
				// Calculate fitted residuals
				for (size_t i = 0; i < n; i++) {
					error_fit[i] = y[i] - y_fit[i];
				}
				// Calculate the standard deviation of each point based on the residual error_fit
				// and nwidth using the MAD robust method
				int nResidual = nwidth - (nwidth / 2) * 2;
				size_t nleftwidth = 0, nrightwidth = 0;
				if (nResidual == 1)		// odd number
				{
					nleftwidth = nwidth / 2;
					nrightwidth = nwidth / 2;
				} else					// even number
				{
					nleftwidth = nwidth / 2 - 1;
					nrightwidth = nwidth / 2;
				}
				// Calculate fitted residuals
				if (n > nwidth) {
					// [ 0, nleftwidth )
					for (size_t i = 0; i < nleftwidth; i++)
						pmad[i] = Mad(error_fit.get(), nwidth);
					// [ n - nrightwidth, n )
					for (size_t i = n - nrightwidth; i < n; i++)
						pmad[i] = Mad(error_fit.get() + n - nwidth, nwidth);
					// [ nleftwidth, n - nrightwidth )
					for (size_t i = nleftwidth; i < n - nrightwidth; i++)
						pmad[i] = Mad(error_fit.get() + i - nleftwidth, nwidth);
				} else {
					// The MAD method needs to be sorted, which is time-consuming to calculate.
					// Here we constrain
					double dMAD = 0;
					if (n <= 500)
						dMAD = Mad(error_fit.get(), n);
					else {
						dMAD = 0;
						for (size_t i = 0; i < n; i++)
							dMAD += error_fit[i] * error_fit[i];
						dMAD = std::sqrt(dMAD / n);
					}
					for (size_t i = 0; i < n; i++)
						pmad[i] = dMAD;
				}
				// In order to identify outliers of "slice",
				// set a reasonable threshold_max threshold_min according to the prior information
				for (size_t i = 0; i < n; i++) {
					if (pmad[i] >= threshold_max)	// Avoid missing outliers
						pmad[i] = threshold_max;
					if (pmad[i] <= threshold_min)
						pmad[i] = threshold_min;	// Avoid good observations being misjudged
				}
				bool bEqual = true;
				n_valid = 0;
				for (size_t i = 0; i < n; i++) {
					// Keep the wild value of the original w
					if (std::fabs(error_fit[i]) >= factor * pmad[i] || w[i] == 0)
						w_new[i] = 0;
					else {
						w_new[i] = 1;
						n_valid++;
					}
					bEqual = bEqual & (w_new[i] == w_old[i]);
				}
				if (bEqual || nLoop > nLoop_max) {
					bResult = true;
					break;
				} else {
					// Update observation weights
					std::copy(w_new.get(), w_new.get() + n, w_old.get());
				}
			}
			// Returns the observation weights, which retains the original outlier markers
			std::copy(w_new.get(), w_new.get() + n, w);
			if (n_valid >= 4)
				return bResult;
			else {
				// Unable to fit, assign y_fit [i] to y [i]
				for (size_t i = 0; i < n; i++)
					y_fit[i] = y[i];
				return false;
			}
		}

		/// <summary> Robust root mean square estimation of zero mean (difference data) data. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/8/22. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="x">	  Zero mean data series. </param>
		/// <param name="marker"> Whether the marker data exceeds the robust threshold, 1-outliers, 0-normal. </param>
		/// <param name="n">	  Number of data. </param>
		/// <param name="factor"> (Optional) Robust control factor, default 6. </param>
		/// <returns> Robust estimation. </returns>
		double RobustStatRms(double x[], double marker[], const size_t n, const double factor = 6.0) _NOEXCEPT {
			double dVar = 0.0;
			for (size_t i = 0; i < n; i++) {
				marker[i] = 0.0;			// All points are considered normal at the initial moment
				dVar = dVar + x[i] * x[i];
			}
			dVar = std::sqrt(dVar / (n - 1));
			while (true) {
#if __cplusplus >= 201103L
				std::unique_ptr<double[]> pQ1(new double[n]());
#else	//__cplusplus < 201103L
				boost::scoped_array<double> pQ1(new double[n]());
#endif  // __cplusplus >= 201103L
				int k = 0;
				double s = 0.0;
				for (size_t i = 0; i < n; i++) {
					if (std::fabs(x[i]) > factor * dVar)
						pQ1[i] = 1;		// Outliers
					else {
						pQ1[i] = 0;
						k++;
						s += x[i] * x[i];
					}
				}
				dVar = std::sqrt(s / (static_cast<double>(k) - 1));
				// Judging pQ1 and pQ0
				bool bfind = false;
				for (size_t i = 0; i < n; i++) {
					if (pQ1[i] != marker[i])
						bfind = true;      // pQ1!=pQ0
					marker[i] = pQ1[i]; // Let pQ0 = pQ1
				}
				if (bfind == false)			// When pQ0 and pQ1 are equal, the iteration ends
					break;
			}
			return dVar;
		}

		/// <summary> Robust root mean square estimation of zero mean (difference data) data. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2008/4/7. </para>
		/// 	<para> Blue Wing, 2020/3/23. </para>
		/// </remarks>
		/// <param name="x">	  The x coordinate. </param>
		/// <param name="n">	  An int to process. </param>
		/// <param name="factor"> (Optional) The factor. </param>
		/// <returns> A double. </returns>
		double RobustStatRms(double x[], const size_t n, const double factor = 6.0) _NOEXCEPT {
#if __cplusplus >= 201103L
			std::unique_ptr<double[]> marker(new double[n]());
#else	//__cplusplus < 201103L
			boost::scoped_array<double> marker(new double[n]());
#endif  // __cplusplus >= 201103L
			return RobustStatRms(x, marker.get(), n, factor);
		}

		/// <summary> Robust stat mean. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2007/8/22. </para>
		/// 	<para> Blue Wing, 2020/3/25. </para>
		/// </remarks>
		/// <param name="x">	  The x coordinate. </param>
		/// <param name="w">	  The width. </param>
		/// <param name="n">	  A size_t to process. </param>
		/// <param name="dMean">  [in,out] The mean. </param>
		/// <param name="dVar">   [in,out] The variable. </param>
		/// <param name="factor"> (Optional) The factor. </param>
		/// <returns> True if it succeeds, false if it fails. </returns>
		bool RobustStatMean(double x[], double w[], const size_t n,
							double& dMean, double& dVar, const double factor = 6.0) _NOEXCEPT {
			dMean = 0;
			for (size_t i = 0; i < n; i++) {
				w[i] = 0;						// All points are considered normal at the initial moment
				dMean += x[i];
			}
			dMean /= n;
			// 计算方差
			dVar = 0;
			for (size_t i = 0; i < n; i++)
				dVar += std::pow(x[i] - dMean, 2);
			dVar = std::sqrt(dVar / (n - 1));

			_CONSTEXPR int nn_max = 10;			// Maximum number of iterations threshold
			int nn = 0;
			while (true) {
				nn++;
				if (nn > nn_max) {
					// Iteration number control
					return false;
				}
				// Outlier judgment based on the relationship between residual size
				// and variance size
#if __cplusplus >= 201103L
				std::unique_ptr<double[]> pw(new double[n]());
#else	//__cplusplus < 201103L
				boost::scoped_array<double> pw(new double[n]());
#endif  // __cplusplus >= 201103L
				for (size_t i = 0; i < n; i++) {
					if (std::fabs(x[i] - dMean) > factor * dVar)
						pw[i] = 1;				// Outlier
					else
						pw[i] = 0;
				}
				// Determine if pw and w are equal
				bool bfind = 0;
				for (size_t i = 0; i < n; i++) {
					if (pw[i] != w[i])
						bfind = 1;   // pw != w
					w[i] = pw[i];
				}

				if (bfind == 0) {
					// When pw and w are equal, the iteration ends
					break;
				}
				// Update mean and variance
				dMean = 0;
				int k = 0;
				for (size_t i = 0; i < n; i++) {
					if (pw[i] == 0) {
						// Normal points are calculated
						k++;
						dMean += x[i];
					}
				}
				dMean /= k;
				dVar = 0;
				for (size_t i = 0; i < n; i++) {
					if (pw[i] == 0) {
						// Normal points are calculated
						dVar += std::pow(x[i] - dMean, 2);
					}
				}
				dVar = std::sqrt(dVar / (static_cast<double>(k) - 1));
			}
			return true;
		}

		/// <summary> Robust polynomial smoothing. </summary>
		/// <remarks>
		///		<para> Gu Defeng, 2009/11/28. </para>
		/// 	<para> Blue Wing, 2020/3/25. </para>
		/// </remarks>
		/// <param name="x">	 Abscissa. </param>
		/// <param name="y">	 Ordinate. </param>
		/// <param name="w">	 Edited weights. </param>
		/// <param name="n">	 Number of data to be smoothed. </param>
		/// <param name="y_fit"> Smooth output value. </param>
		/// <param name="m">	 (Optional) Polynomial order, 2 = <m <= n, default is 3. </param>
		/// <returns> True if it succeeds, false if it fails. </returns>
		bool RobustPolyFit(double x[], double y[], double w[],
						   const size_t n, double y_fit[], const size_t m = 3) _NOEXCEPT {
#if __cplusplus >= 201103L
			std::unique_ptr<double[]> w_new(new double[n]());
#else	//__cplusplus < 201103L
			boost::scoped_array<double> w_new(new double[n]());
#endif  // __cplusplus >= 201103L
			for (size_t i = 0; i < n; i++) {
				w[i] = 1.0;
				w_new[i] = 1.0;
			}

			if (m < 1 || m > n) {
				return false;
			}

			int nLoop = 0;
			_CONSTEXPR int nLoop_max = 6; // 设置一个迭代次数阈值，避免迭代在临界处震荡，无法收敛
			while (true) {
				nLoop++;
				Matrix matC(n, m);
				Matrix matY(n, 1);
#if __cplusplus >= 201103L
				std::unique_ptr<double[]> xx(new double[n]());
#else	//__cplusplus < 201103L
				boost::scoped_array<double> xx(new double[n]());
#endif  // __cplusplus >= 201103L
				for (size_t i = 0; i < n; i++)
					xx[i] = x[i] - x[0];
				for (size_t i = 0; i < n; i++) {
					matY(i, 0) = w[i] * y[i];
					matC(i, 0) = w[i] * 1.0;
					for (size_t j = 1; j < m; j++)
						matC(i, j) = w[i] * pow(xx[i], j);
				}
				Matrix matS = Matrix(matC.Transpose() * matC).Inv() * matC.Transpose() * matY;
				Matrix matY_Fit = matC * matS;
				// 计算均方根
				double rms = 0;
				int kk = 0;
				for (size_t i = 0; i < n; i++) {
					y_fit[i] = matY_Fit(i, 0);
					if (w[i] == 1.0) {
						kk++;
						rms += std::pow(y[i] - y_fit[i], 2);
					}
				}
				rms = std::sqrt(rms / kk);
				bool bEqual = true;
				for (size_t i = 0; i < n; i++) {
					if (std::fabs(y[i] - y_fit[i]) >= 3.0 * rms)
						w_new[i] = 0;
					else
						w_new[i] = 1;
					bEqual = bEqual & (w_new[i] == w[i]);
				}
				if (bEqual || nLoop > nLoop_max) {
					break;
				} else {
					std::copy(w_new.get(), w_new.get() + n, w);
				}
			}
			return true;
		}

		/// <summary> Random normal. </summary>
		/// <remarks> Blue Wing, 2020/3/25. </remarks>
		/// <param name="mu">    (Optional) The mean value, default is 0. </param>
		/// <param name="sigma"> (Optional) The variance, default is 1.0. </param>
		/// <returns> A double. </returns>
		double RandNormal(const double mu = 0.0, const double sigma = 1.0) _NOEXCEPT {
#if __cplusplus >= 201103L
			std::mt19937 generator;
			std::normal_distribution distribution(mu, std::sqrt(sigma));
#else
			boost::mt19937 generator;
			boost::normal_distribution distribution(mu, std::sqrt(sigma));
#endif // __cplusplus >= 201103L

			return distribution(generator);
		}

	}	// namespace Math
}	// namespace NUDTTK

#endif	// #ifndef _NUDTTK_MATH_ALGORITHM_TR_