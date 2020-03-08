#pragma once

#include <cmath>
#include <numeric>
#include <algorithm>
#include <functional>

namespace NUDTTK
{
	namespace Math
	{
		/// <summary> Rounds the given number. </summary>
		/// <remarks> Blue Wing, 2020/3/8. </remarks>
		/// <param name="x"> The number to be rounded. </param>
		/// <returns> The rounded number. </returns>
		double Round(const double x)
		{
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
		constexpr double factorial(const size_t n)
		{
			return n == 0 ? 1 : n * factorial(n - 1);
		}
#else	// __cplusplus != 201103L
#if __cplusplus > 201103L
		constexpr double Factorial(const size_t n)
#else	// __cplusplus < 201103L
		double Factorial(const size_t n)
#endif	// __cplusplus > 201103L
		{
			// The odd number multiplication table
			// Store PI(i=1,n){ 2 * i - 1 } from n = 1
			double odd_multiplication_table[32] = {
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
			double pow2_table[32] = {
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
			for (; shift_idx != 1; odd_table_idx >>= 1)
			{
				factorial_num *= odd_multiplication_table[odd_table_idx - 1];
				odd_table_idx = shift_idx - odd_table_idx + 1;
				shift_idx >>= 1;
				shift_num += shift_idx;
			}

			factorial_num *= pow2_table[shift_num];

			return factorial_num;
		}
#endif	// __cplusplus == 201103L

		/// <summary> Sort number list with given size and order. </summary>
		/// <remarks> Blue Wing, 2020/3/8. </remarks>
		/// <typeparam name="T"> Numerical type </typeparam>
		/// <param name="sort_list"> The number list to be sorted. </param>
		/// <param name="list_size"> Size of x list. </param>
		/// <param name="ascend">    True to ascend. </param>
		template<typename T>
		void Sort(T sort_list[], const size_t list_size, const bool ascend = true)
		{
			if (ascend)
			{
				// ASC
				std::sort(sort_list, sort_list + list_size, std::less<T>());
			}
			else
			{
				// DESC
				std::sort(sort_list, sort_list + list_size, std::greater<T>());
			}
		}

		/// <summary> 
		/// 	<para> Sort number list with given size and order. </para>
		///		<para> Ensure that the same values ​​isotonic order in list. </para>
		///	</summary>
		/// <remarks> Blue Wing, 2020/3/8. </remarks>
		/// <typeparam name="T"> Numerical type </typeparam>
		/// <param name="sort_list">	   The number list to be sorted. </param>
		/// <param name="sort_index_list"> The sequence of number list(should remain uninitialized). </param>
		/// <param name="list_size">	   Size of x list. </param>
		/// <param name="ascend">		   True to ascend. </param>
		template<typename T>
		void Sort(T sort_list[], size_t sort_index_list[], const size_t list_size, const bool ascend = true)
		{
			// Initialize original index locations
			std::iota(sort_index_list, sort_index_list + list_size, 0);

			if (ascend)
			{
				// ASC
#if __cplusplus >= 201103L
				std::stable_sort(sequence, sequence + n,
					[&x](size_t i1, size_t i2) {return x[i1] < x[i2]; });
#else
				// Callable functor
				struct
				{
					T* sort_list;
					bool operator()(size_t i1, size_t i2) const
					{
						return sort_list[i1] < sort_list[i2];
					}
				} Less;
				Less.sort_list = sort_list;
				std::stable_sort(sort_index_list, sort_index_list + list_size, Less);
#endif
				std::sort(sort_list, sort_list + list_size, std::less<T>());
			}
			else
			{
				// DESC
#if __cplusplus >= 201103L
				std::stable_sort(sort_index_list, sort_index_list + list_size,
					[&sort_list](size_t i1, size_t i2) {return sort_list[i1] > sort_list[i2]; });
#else
				// Callable functor
				struct
				{
					T* sort_list;
					bool operator()(size_t i1, size_t i2) const
					{
						return sort_list[i1] > sort_list[i2];
					}
				} Greater;
				Greater.sort_list = sort_list;
				std::stable_sort(sort_index_list, sort_index_list + list_size, Greater);
#endif
				std::sort(sort_list, sort_list + list_size, std::greater<T>());
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
		void LegendreFunc(long double** P, const size_t n, const double u)
		{
			const double cosu = std::sqrt(1 - u * u);
			for (size_t i = 0; i <= n; i++)
			{
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
			for (size_t i = 3; i <= n; i++)
			{
				for (size_t j = 1; j <= i - 2; j++)
				{
					P[i][j] = std::sqrt((4.0 * i * i - 1.0) / (static_cast<double>(i)* i - static_cast<double>(j)* j))* u* P[i - 1][j]
						- std::sqrt(((2.0 * i + 1.0) * (i - 1.0 + j) * (i - 1.0 - j)) / ((2.0 * i - 3.0) * (static_cast<double>(i) + j)* (i - j)))* P[i - 2][j];
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
		void LegendreFuncDerivative(long double** P, long double** DP, const size_t n, const double u)
		{
			// First calculate the Legendre function value P
			LegendreFunc(P, n, u);
			// Recursively calculate the derivative value of Legendre function DP
			double cosu = std::sqrt(1.0 - u * u);
			for (size_t i = 0; i <= n; i++)
			{
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
					DP[i][j] = std::sqrt((4.0 * i * i - 1.0) / (static_cast<double>(i)* i - static_cast<double>(j)* j))* (u * DP[i - 1][j] + cosu * P[i - 1][j])
					- std::sqrt(((2.0 * i + 1.0) * (i - 1.0 + j) * (i - 1.0 - j)) / ((2.0 * i - 3.0) * (static_cast<double>(i) + j)* (static_cast<double>(i) - j)))* DP[i - 2][j];
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
		constexpr double Combination(const size_t n, const size_t m)
#else
		double Combination(const size_t n, const size_t m)
#endif
		{
			// [NOTE]
			// Actually, we should use Dynamic Programming (DP) to solve this for better performance,
			// since our Factorial function is fast enough, we don't need this
			return Factorial(n) / (Factorial(n - m) * Factorial(m));
		}

		//void   InterploationLagrange(double xa[], double ya[], int n, double x, double& y);
		//void   InterploationLagrange(double xa[], double ya[], int n, double x, double& y, double& dy);
		//double Median(double x[], int n);
		//double Mad(double x[], int n);
		//bool   PolyFit(double x[], double y[], int n, double y_fit[], int m = 3);
		//bool   VandrakFilter(double x[], double y[], double w[], int n, double eps, double y_fit[]);
		//bool   KinematicRobustVandrakFilter(double x[], double y[], double w[], int n, double dEPS, double y_fit[], double threshold_max, double threshold_min, int nwidth, double factor = 3);		// 鲁棒计算均方根
		//double RobustStatRms(double x[], int n, double factor = 6);
		//double RobustStatRms(double x[], double marker[], int n, double factor = 6);
		//bool   RobustStatMean(double x[], double w[], int n, double& dMean, double& dVar, double factor = 6);
		//bool   RobustPolyFit(double x[], double y[], double w[], int n, double y_fit[], int m = 3);
		//double RandNormal(double mu = 0, double sigma = 1.0);
	}
}