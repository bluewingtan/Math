#pragma once

#ifndef _NUDTTK_MATH_MATRIX_TR_
#define _NUDTTK_MATH_MATRIX_TR_

#include "common.h"

#include <map>
#include <string>
#include <sstream>

// Check compiler version required
#ifdef _MSC_VER				// MSVC
#if _MSC_VER < 1500
#error You need to compile this file with Visual Studio 2008 or above.
#elif _MSC_VER >= 1500 && _MSC_VER < 1800
// Just for legacy MSVC compiler
#include <EigenLegacy/Dense>
#elif _MSC_VER >= 1800
#include <Eigen/Dense>
#endif	// _MSC_VER < 1500
#elif defined __GNUC__		// GCC
#if __GNUC__ < 4 && __GNUC_MINOR__ < 8
#error You need to compile this file with GCC 4.8 or above.
#else
#include <Eigen/Dense>
#endif	// __GNUC__ < 4 && __GNUC_MINOR__ < 8
#elif defined __clang__		// llvm
#if __clang_major__ < 3 && __clang_minor__ < 4
#error You need to compile this file with clang(llvm) 3.4 or above.
#else
#include <Eigen/Dense>
#endif	// __clang_major__ < 3 && __clang_minor__ < 4
#elif __cplusplus < 201103L	// Other compiler
#error You need to compile this file with compilers support C++ 11.
#else						// Other compiler support C++ 11
#include <Eigen/Dense>
#endif	// #ifdef _MSC_VER

// Using optional to implement Lazy-Evaluation
#if __cplusplus >= 201103L
#include <utility>
#include <functional>
#include <type_traits>
#include <algorithm>
#if __cplusplus >= 201703L
#include <optional>
#else	// __cplusplus < 201703L && __cplusplus >= 201103L
#include <boost/optional.hpp>
#endif	// __cplusplus >= 201703L
#else	// __cplusplus < 201103L
#define NOT_SUPPORT_LAZY_EVALUATION	// Not support lazy evaluation
#include <boost/type_traits.hpp>
#endif	// __cplusplus >= 201103L

namespace NUDTTK {

	template<typename _T>
	struct _scalar_support;

	template<typename _T>
	struct _non_scalar_support {};

	template<typename _T>
	class Matrix;

	/// <summary> A macro that defines modern (above C++11) unwrap binary operation. </summary>
	/// <remarks> Blue Wing, 2020/3/14. </remarks>
	/// <param name="_operator"> The operator. </param>
#define BINARY_OP_UNWRAP_MODERN(_operator)										\
public: template<typename _Result>												\
_CONSTEXPR_FN _Result unwrap() const _NOEXCEPT {								\
	return lhs_.unwrap<_Result>() _operator rhs_.unwrap<_Result>();				\
}

/// <summary> A macro that defines legacy (below C++11) unwrap binary operation. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_operator"> The operator. </param>
#define BINARY_OP_UNWRAP_LEGACY(_operator)										\
public: template<typename _Result>												\
typename _Result unwrap() const {												\
	return lhs_.unwrap<_Result>() _operator rhs_.unwrap<_Result>();				\
}

/// <summary> A macro that defines modern (above C++11) binary operation unused constructor. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_impl"> The implement. </param>
#define BINARY_OP_UNUSED_CONSTRUCTOR_MODERN(_impl)								\
_CONSTEXPR_FN _impl##() _NOEXCEPT = delete;

/// <summary> A macro that defines legacy (below C++11) binary operation unused constructor. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_impl"> The implement. </param>
#define BINARY_OP_UNUSED_CONSTRUCTOR_LEGACY(_impl)								\
private: _impl##(){};

// Check support status
#ifdef NOT_SUPPORT_LAZY_EVALUATION
#define BINARY_OP_UNWRAP BINARY_OP_UNWRAP_LEGACY
#define BINARY_OP_UNUSED_CONSTRUCTOR BINARY_OP_UNUSED_CONSTRUCTOR_LEGACY
#define CLS_UNUSED_CONSTRUCTOR BINARY_OP_UNUSED_CONSTRUCTOR_LEGACY
#else 
#define BINARY_OP_UNWRAP BINARY_OP_UNWRAP_MODERN
#define BINARY_OP_UNUSED_CONSTRUCTOR BINARY_OP_UNUSED_CONSTRUCTOR_MODERN
#define CLS_UNUSED_CONSTRUCTOR BINARY_OP_UNUSED_CONSTRUCTOR_MODERN
#endif

/// <summary> A macro that defines binary operation implement. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_impl"> The implementation name. </param>
/// <param name="_operator">  The operator. </param>
#define CREATE_BINARY_OP_IMPL(_impl, _operator)									\
template<typename _Lhs, typename _Rhs>											\
class _impl {																	\
public:																			\
	_CONSTEXPR_FN _impl##(_Lhs const& lhs, _Rhs const& rhs) _NOEXCEPT			\
		: lhs_(lhs), rhs_(rhs) {}												\
	BINARY_OP_UNUSED_CONSTRUCTOR(_impl)											\
	BINARY_OP_UNWRAP(_operator)													\
private:																		\
	const _Lhs& lhs_;															\
	const _Rhs& rhs_;															\
};

/// <summary> A macro that defines combine Implementation with binary Operation. </summary>
/// <remarks> Blue Wing, 2020/3/20. </remarks>
/// <param name="_impl">	 The implementation. </param>
/// <param name="_operator"> The operator. </param>
/// <param name="_scalar">   The scalar. </param>
/// <param name="_wrapped">  The wrapped. </param>
#ifdef _TYPE_TRAITS_
#define COMBINE_IMPL_WITH_BINARY_OP(_impl, _operator, _scalar, _wrapped)					\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename std::enable_if<std::is_arithmetic<_Element>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>							\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,									\
						 const _Element& rhs) _NOEXCEPT {									\
	return _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>(lhs, rhs);					\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename std::enable_if<!std::is_arithmetic<_Element>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _Element>										\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,									\
						 const _Element& rhs) _NOEXCEPT {									\
	return _impl##<##_impl##<_Lhs, _Rhs>, _Element>(lhs, rhs);								\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename std::enable_if<std::is_arithmetic<_Element>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>							\
	operator _operator##(const _Element& lhs,												\
						 const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {						\
	return _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>(lhs, rhs);					\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename std::enable_if<!std::is_arithmetic<_Element>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<_Element, _impl##<_Lhs, _Rhs>>										\
	operator _operator##(const _Element& lhs,												\
						 const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {						\
	return _impl##<_Element, _impl##<_Lhs, _Rhs>>(lhs, rhs);								\
}																							\
template<typename _Lhs, typename _T,														\
		 typename std::enable_if<std::is_arithmetic<_Lhs>::value, int>::type = 0>			\
_CONSTEXPR_FN _impl##<##_scalar##<_Lhs>, _wrapped##<_T>>									\
	operator _operator##(const _Lhs& lhs, const _wrapped##<_T>& rhs) _NOEXCEPT {			\
	return _impl##<##_scalar##<_Lhs>, _wrapped##<_T>>(lhs, rhs);							\
}																							\
template<typename _Rhs, typename _T,														\
		 typename std::enable_if<std::is_arithmetic<_Rhs>::value, int>::type = 0>			\
_CONSTEXPR_FN _impl##<##_wrapped##<_T>, _scalar##<_Rhs>>									\
	operator _operator##(const _wrapped##<_T>& lhs, const _Rhs& rhs) _NOEXCEPT {			\
	return _impl##<_wrapped##<_T>, _scalar##<_Rhs>>(lhs, rhs);								\
}																							\
template<typename _T>																		\
_CONSTEXPR_FN _impl##<##_wrapped##<_T>, _wrapped##<_T>>										\
	operator _operator##(const _wrapped##<_T>& lhs,											\
						 const _wrapped##<_T>& rhs) _NOEXCEPT {								\
	return _impl##<##_wrapped##<_T>, _wrapped##<_T>>(lhs, rhs);								\
}
#else
#define COMBINE_IMPL_WITH_BINARY_OP(_impl, _operator, _scalar, _wrapped)					\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<boost::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>							\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,									\
						 const _Element& rhs) _NOEXCEPT {									\
	return _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>(lhs, rhs);					\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<!boost::is_arithmetic<_Element>::value, int>::type = 0>\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _Element>										\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,									\
						 const _Element& rhs) _NOEXCEPT {									\
	return _impl##<##_impl##<_Lhs, _Rhs>, _Element>(lhs, rhs);								\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<boost::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>							\
	operator _operator##(const _Element& lhs,												\
						 const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {						\
	return _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>(lhs, rhs);					\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<!boost::is_arithmetic<_Element>::value, int>::type = 0>\
_CONSTEXPR_FN _impl##<_Element, _impl##<_Lhs, _Rhs>>										\
	operator _operator##(const _Element& lhs,												\
						 const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {						\
	return _impl##<_Element, _impl##<_Lhs, _Rhs>>(lhs, rhs);								\
}																							\
template<typename _Lhs, typename _T,														\
		 typename boost::enable_if_c<boost::is_arithmetic<_Lhs>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<##_scalar##<_Lhs>, _wrapped##<_T>>									\
	operator _operator##(const _Lhs& lhs, const _wrapped##<_T>& rhs) _NOEXCEPT {			\
	return _impl##<##_scalar##<_Lhs>, _wrapped##<_T>>(lhs, rhs);							\
}																							\
template<typename _Rhs, typename _T,														\
		 typename boost::enable_if_c<boost::is_arithmetic<_Rhs>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<##_wrapped##<_T>, _scalar##<_Rhs>>									\
	operator _operator##(const _wrapped##<_T>& lhs, const _Rhs& rhs) _NOEXCEPT {			\
	return _impl##<_wrapped##<_T>, _scalar##<_Rhs>>(lhs, rhs);								\
}																							\
template<typename _T>																		\
_CONSTEXPR_FN _impl##<##_wrapped##<_T>, _wrapped##<_T>>										\
	operator _operator##(const _wrapped##<_T>& lhs,											\
						 const _wrapped##<_T>& rhs) _NOEXCEPT {								\
	return _impl##<##_wrapped##<_T>, _wrapped##<_T>>(lhs, rhs);								\
}
#endif // _TYPE_TRAITS_

/// <summary> A macro that defines create and combine binary operation. </summary>
/// <remarks> Blue Wing, 2020/3/15. </remarks>
/// <param name="_impl">	 The implementation name. </param>
/// <param name="_operator"> The operator. </param>
/// <param name="_scalar">   The scalar. </param>
/// <param name="_wrapped">  The wrapped. </param>
#define CREATE_AND_COMBINE_BINARY_OP(_impl, _operator, _scalar, _wrapped)		\
CREATE_BINARY_OP_IMPL(_impl, _operator)											\
COMBINE_IMPL_WITH_BINARY_OP(_impl, _operator, _scalar, _wrapped)

/// <summary> A macro that defines class member binary operation function. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_class_name">    Name of the class. </param>
/// <param name="_return_type">   Type of the return. </param>
/// <param name="_variable_name"> Name of the variable. </param>
/// <param name="_impl">	  The implement. </param>
#define CLS_BINARY_OP(_class_name, _return_type, _variable_name, _impl)			\
template<typename Lhs, typename Rhs>											\
_CONSTEXPR_FN _class_name##(const _impl##<Lhs, Rhs>& op) _NOEXCEPT {			\
	_variable_name = op.unwrap<##_return_type##>();								\
}																				\
template<typename Lhs, typename Rhs>											\
_CONSTEXPR_FN _class_name##& operator=(const _impl##<Lhs, Rhs>& op) _NOEXCEPT {	\
	_variable_name = op.unwrap<##_return_type##>();								\
	return *this;																\
}

/// <summary> A macro that defines class unwrap member binary operation. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_class_name">    Name of the class. </param>
/// <param name="_return_type">   Type of the return. </param>
/// <param name="_variable_name"> Name of the variable. </param>
#define CLS_UNWRAP(_class_name, _return_type, _variable_name)					\
template<typename _U = _return_type##>											\
_CONSTEXPR_FN _U unwrap() const _NOEXCEPT {										\
	return _variable_name##;													\
}

	template<typename _T>
	struct _scalar_support {
		_T value_;

		_CONSTEXPR_FN _scalar_support(const _T& value) _NOEXCEPT
			: value_(value) {}
		CLS_UNUSED_CONSTRUCTOR(_scalar_support)

			template<typename _Unused>
		_CONSTEXPR_FN _T unwrap() const _NOEXCEPT {
			return value_;
		}
	};

	// Support operator + - with Matrix, * for both Matrix and scalar
	CREATE_AND_COMBINE_BINARY_OP(add_op_impl, +, _non_scalar_support, Matrix);
	CREATE_AND_COMBINE_BINARY_OP(sub_op_impl, -, _non_scalar_support, Matrix);
	CREATE_AND_COMBINE_BINARY_OP(mul_op_impl, *, _scalar_support, Matrix);
	CREATE_AND_COMBINE_BINARY_OP(div_op_impl, /, _scalar_support, Matrix);

	// Epsilon value when check equality
	_CONSTEXPR_FN double epsilon = 1e-7;

	/// <summary> A wrapper class for Eigen to support. </summary>
	/// <remarks> Blue Wing, 2020/3/14. </remarks>
	/// <typeparam name="_T"> Type of the t. </typeparam>
	template<typename _T>
	class Matrix {
	public:
		typedef Eigen::Matrix<_T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> base_type;
		CLS_BINARY_OP(Matrix, base_type, value_, add_op_impl);
		CLS_BINARY_OP(Matrix, base_type, value_, sub_op_impl);
		CLS_BINARY_OP(Matrix, base_type, value_, mul_op_impl);
		CLS_BINARY_OP(Matrix, base_type, value_, div_op_impl);
		CLS_UNWRAP(Matrix, base_type, value_);

	public:
		/// <summary> Default constructor (no thing to do). </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		_CONSTEXPR_FN Matrix() _NOEXCEPT {}

		/// <summary> Initialize Matrix in row size and col size. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="row_size"> Size of the row. </param>
		/// <param name="col_size"> Size of the col. </param>
		_CONSTEXPR_FN Matrix(const size_t row_size, const size_t col_size) _NOEXCEPT
			: value_(base_type::Zero(row_size, col_size)) {}

		/// <summary> Initialize Matrix in row size and col size with default values. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="row_size">	Size of the row. </param>
		/// <param name="col_size">	The cols. </param>
		/// <param name="default_values"> 
		/// 	<para> The default values. </para>
		/// 	<para> The caller MUST ensure that the size of the number is the same as the
		/// 	parameter passed in, otherwise a serious problem of out-of-bounds memory reads and writes
		/// 	will occur. </para>
		/// </param>
		_CONSTEXPR_FN Matrix(const size_t row_size, const size_t col_size, _T default_values[])
			: value_(Eigen::Map<base_type>(default_values, row_size, col_size)) {}

		/// <summary> Initialize square Matrix in square size. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="edge_size"> Edge size of the square. </param>
		_CONSTEXPR_FN Matrix(const size_t edge_size) _NOEXCEPT
			: value_(base_type::Zero(edge_size, edge_size)) {}

		/// <summary> Initialize square Matrix in square size with default values. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="edge_size"> Size of the square. </param>
		/// <param name="default_values"> 
		/// 	<para> The default values. </para>
		/// 	<para> The caller MUST ensure that the size of the number is the same as the
		/// 	parameter passed in, otherwise a serious problem of out-of-bounds memory reads and writes
		/// 	will occur. </para>
		/// </param>
		_CONSTEXPR_FN Matrix(const size_t edge_size, _T default_values[])
			: value_(Eigen::Map<base_type>(default_values, edge_size, edge_size)) {}

		/// <summary> Copy constructor. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="other"> Other Matrix instance. </param>
		_CONSTEXPR_FN Matrix(const Matrix<_T>& other) _NOEXCEPT
			: value_(other.value_)
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			, lazy_value_map_(other.lazy_value_map_)
#endif // !NOT_SUPPORT_LAZY_EVALUATION
		{}

		/// <summary> Copy assignment operator. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="other"> Other Matrix instance. </param>
		/// <returns> A shallow copy of this. </returns>
		_CONSTEXPR_FN Matrix<_T>& operator=(const Matrix<_T>& other) _NOEXCEPT {
			value_ = other.value_;

#ifndef NOT_SUPPORT_LAZY_EVALUATION
			lazy_value_map_ = other.lazy_value_map_;
#endif // !NOT_SUPPORT_LAZY_EVALUATION
			return *this;
		}

		// Moving constructor and operator
#if __cplusplus >= 201103L
		/// <summary> Move constructor. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="other"> Other to be MOVED Matrix instance. </param>
		_CONSTEXPR_FN Matrix(Matrix<_T>&& other) _NOEXCEPT
			: value_(std::move(other.value_))
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			, lazy_value_map_(std::move(other.lazy_value_map_))
#endif // !NOT_SUPPORT_LAZY_EVALUATION
		{}

		/// <summary> Move assignment operator. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="other"> Other to be MOVED Matrix instance. </param>
		/// <returns> A shallow copy of this. </returns>
		_CONSTEXPR_FN Matrix<_T>& operator=(Matrix<_T>&& other) _NOEXCEPT {
			value_ = std::move(other.value_);

#ifndef NOT_SUPPORT_LAZY_EVALUATION
			lazy_value_map_ = std::move(other.lazy_value_map_);
#endif // !NOT_SUPPORT_LAZY_EVALUATION
			return *this;
		}
#endif	// __cplusplus >= 201103L

		~Matrix() _NOEXCEPT {}

	public:
		/// <summary>
		/// 	<para> So called initializes. </para>
		///		<para> Do not use this function to initialize Matrix, consider constructor to instead. </para>
		///		<para> This function should be renamed to <c>resize</c>. </para>
		/// </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="row_size"> Size of the row. </param>
		/// <param name="col_size"> Size of the col. </param>
		/// <returns> Intentionally always return true. </returns>
		_CONSTEXPR_FN bool Init(const size_t row_size, const size_t col_size) _NOEXCEPT {
			// Move constructor
			value_ = base_type::Zero(row_size, col_size);

			// Should update lazy evaluation values
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			std::for_each(lazy_value_map_.begin(), lazy_value_map_.end(),
						  [&](value_t& lazy_value) {lazy_value.reset(); });
#endif // !NOT_SUPPORT_LAZY_EVALUATION

			return true;
		}

		/// <summary> Makes unit matrix. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="edge_size"> Size of the edge. </param>
		/// <returns> Intentionally always return true. </returns>
		_CONSTEXPR_FN bool MakeUnitMatrix(const size_t edge_size) _NOEXCEPT {
			// Move constructor
			value_ = base_type::Identity(edge_size, edge_size);

			// Should update lazy evaluation values
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			std::for_each(lazy_value_map_.begin(), lazy_value_map_.end(),
						  [&](value_t& lazy_value) {lazy_value.reset(); });
#endif // !NOT_SUPPORT_LAZY_EVALUATION

			return true;
		}

		/// <summary> Convert this into a string representation. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="delim">	  (Optional) The delimiter. </param>
		/// <param name="line_break"> (Optional) True to line break. </param>
		/// <returns> A std::string that represents this. </returns>
		std::string ToString(const std::string& delim = " ", const bool line_break = true) _NOEXCEPT {
			Eigen::IOFormat print_format(Eigen::FullPrecision, 0, delim, line_break ? "\n" : "");
			std::stringstream ss;
			ss << value_.format(print_format);
			return ss.str();
		}

	public:
		/// <summary> Equality operator. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <param name="other"> The other. </param>
		/// <returns> True if the parameters are considered equivalent. </returns>
		_CONSTEXPR_FN bool operator ==(const Matrix<_T>& other) const _NOEXCEPT {
			if (other.value_.isZero()) {
				// According to Eigen document, when other is Zero matrix, should use isMuchSmallerThan
				// and given epsilon value
				return value_.isMuchSmallerThan(other, epsilon);
			} else {
				// For matrices, the comparison is done using the Hilbert-Schmidt norm
				return value_.isApprox(other);
			}
		}

		/// <summary> Inequality operator. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <param name="other"> The other. </param>
		/// <returns> True if the parameters are not considered equivalent. </returns>
		_CONSTEXPR_FN bool operator !=(const Matrix<_T>& other) const _NOEXCEPT {
			return !(*this == other);
		}

		/// <summary> Sets an element. </summary>
		/// <remarks> Blue Wing, 2020/3/15. </remarks>
		/// <param name="row_index"> The row index. </param>
		/// <param name="col_index"> The col index. </param>
		/// <param name="value">	 The value. </param>
		/// <returns> Intentionally always return true. </returns>
		_CONSTEXPR_FN bool SetElement(const size_t row_index, const size_t col_index, _T value) _NOEXCEPT {
			value_(row_index, col_index) = value;

			// Should update lazy evaluation values
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			std::for_each(lazy_value_map_.begin(), lazy_value_map_.end(),
						  [&](value_t& lazy_value) {lazy_value.reset(); });
#endif // !NOT_SUPPORT_LAZY_EVALUATION

			return true;
		}

		/// <summary> Get particular item by index. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		/// <param name="row_index"> Zero-based index of the row index. </param>
		/// <param name="col_index"> Zero-based index of the col index. </param>
		_CONSTEXPR_FN _T GetElement(const size_t row_index, const size_t col_index) const _NOEXCEPT {
			return value_(row_index, col_index);
		}

		/// <summary> Gets number columns. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		/// <returns> The number columns. </returns>
		_CONSTEXPR_FN size_t GetNumColumns() const _NOEXCEPT {
			return static_cast<size_t>(value_.cols());
		}

		/// <summary> Gets number rows. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		/// <returns> The number rows. </returns>
		_CONSTEXPR_FN size_t GetNumRows() const _NOEXCEPT {
			return static_cast<size_t>(value_.rows());
		}

		/// <summary> Get the max item and coordinates. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		/// <param name="row_index"> [in,out] Zero-based index of the row. </param>
		/// <param name="col_index"> [in,out] Zero-based index of the col. </param>
		_CONSTEXPR_FN _T Max(size_t& row_index, size_t& col_index) const _NOEXCEPT {
			return value_.maxCoeff(&row_index, &col_index);
		}

		/// <summary> Get the max item. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		_CONSTEXPR_FN _T Max() const _NOEXCEPT {
			return value_.maxCoeff();
		}

		/// <summary> Get the min item and coordinates. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		/// <param name="nRow"> [in,out] The row. </param>
		/// <param name="nCol"> [in,out] The col. </param>
		_CONSTEXPR_FN _T Min(size_t& row_index, size_t& col_index) const _NOEXCEPT {
			return value_.minCoeff(&row_index, &col_index);
		}

		/// <summary> Get the min item. </summary>
		/// <remarks> Blue Wing, 2020/3/16. </remarks>
		_CONSTEXPR_FN _T Min() const _NOEXCEPT {
			return value_.minCoeff();
		}

	public:

		/// <summary> Gets the transpose. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <returns> A Matrix&lt;_T&gt; </returns>
		_CONSTEXPR_FN Matrix<_T> Transpose() const _NOEXCEPT {
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			auto lazy_value = lazy_value_map_.at("tranpose_value");
			if (!lazy_value) {
				lazy_value = value_.transpose();
			}
			return Matrix<_T>(lazy_value.value());
#else
			return Matrix<_T>(value_.transpose());
#endif // !NOT_SUPPORT_LAZY_EVALUATION
		}

		/// <summary> Gets the abs. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <returns> A Matrix&lt;_T&gt; </returns>
		_CONSTEXPR_FN Matrix<_T> Abs() const _NOEXCEPT {
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			auto lazy_value = lazy_value_map_.at("absolute_value");
			if (!lazy_value) {
				lazy_value = value_.cwiseAbs();
			}
			return Matrix<_T>(lazy_value.value());
#else
			return Matrix<_T>(value_.cwiseAbs());
#endif // !NOT_SUPPORT_LAZY_EVALUATION
		}

		/// <summary> Gets the inverse. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <returns> A Matrix&lt;_T&gt; </returns>
		_CONSTEXPR_FN Matrix<_T> Inv() const _NOEXCEPT {
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			auto lazy_value = lazy_value_map_.at("inverse_value");
			if (!lazy_value) {
				Eigen::FullPivLU<base_type> lu(value_);
				if (lu.isInvertible()) {
					lazy_value = value_.inverse();
					return Matrix<_T>(lazy_value.value());
				} else {
					return Matrix<_T>();
				}
			}
#else
			Eigen::FullPivLU<base_type> lu(value_);
			if (lu.isInvertible()) {
				return Matrix<_T>(value_.inverse());
			} else {
				return Matrix<_T>();
			}
#endif // !NOT_SUPPORT_LAZY_EVALUATION
		}

		/// <summary> Inverse ssgj. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <returns> A Matrix&lt;_T&gt; </returns>
		_CONSTEXPR_FN Matrix<_T> Inv_Ssgj() const _NOEXCEPT {
			return Inv();
		}

		/// <summary> Get matrix determinant value. </summary>
		/// <remarks> Blue Wing, 2020/3/21. </remarks>
		/// <returns> Determinant value </returns>
		_CONSTEXPR_FN _T DetGauss() const _NOEXCEPT {
#ifndef NOT_SUPPORT_LAZY_EVALUATION
			auto lazy_value = lazy_value_map_.at("determinant_value");
			if (!lazy_value) {
				lazy_value = value_.determinant();
			}
			return Matrix<_T>(lazy_value.value());
#else
			return Matrix<_T>(value_.determinant());
#endif // !NOT_SUPPORT_LAZY_EVALUATION
		}

	private:
		base_type value_;	// The matrix value
#ifndef NOT_SUPPORT_LAZY_EVALUATION
#ifdef _OPTIONAL_
		using value_t = std::optional<Matrix<_T>::base_type>;
#else	// BOOST_OPTIONAL_FLC_19NOV2002_HPP
		using value_t = boost::optional<Matrix<_T>::base_type>;
#endif	// _OPTIONAL_
		std::map<std::string, value_t> lazy_value_map_{
			{"tranpose_value", value_t()},
			{"absolute_value", value_t()},
			{"inverse_value", value_t()},
			{"determinant_value", value_t()},
		};
#endif	// !NOT_SUPPORT_LAZY_EVALUATION
	};
}


#endif	// #ifndef _NUDTTK_MATH_MATRIX_TR_