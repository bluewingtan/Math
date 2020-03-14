#pragma once

#ifndef _NUDTTK_MATH_MATRIX_TR_
#define _NUDTTK_MATH_MATRIX_TR_

#include "common.h"

#include <string>

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
	// Forward declarations
	template<typename _T>
	class Matrix;

	// Return type traits
#ifndef NOT_SUPPORT_LAZY_EVALUATION
	// Main template
	template <typename _F>
	struct _return_type_impl;

	// Partial specialization - regular functions
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args...)> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args..., ...)> { using type = _R; };

	// Partial specialization - regular functions l-reference
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args...)&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args..., ...)&> { using type = _R; };

	// Partial specialization - regular functions r-reference
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args...)&&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args..., ...)&&> { using type = _R; };

	// Partial specialization - volatile regular functions
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args...) volatile> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args..., ...) volatile> { using type = _R; };

	// Partial specialization - volatile regular functions l-reference
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args...) volatile&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args..., ...) volatile&> { using type = _R; };

	// Partial specialization - volatile regular functions r-reference
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args...) volatile&&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(_Args..., ...) volatile&&> { using type = _R; };

	// Partial specialization - regular function pointers
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(*)(_Args...)> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(*)(_Args..., ...)> { using type = _R; };

	// Partial specialization - regular function l-references
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&)(_Args...)> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&)(_Args..., ...)> { using type = _R; };

	// Partial specialization - regular function l-references l-references
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&)(_Args...)&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&)(_Args..., ...)&> { using type = _R; };

	// Partial specialization - regular function l-references r-references
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&)(_Args...)&&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&)(_Args..., ...)&&> { using type = _R; };

	// Partial specialization - regular function r-references
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&&)(_Args...)> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&&)(_Args..., ...)> { using type = _R; };

	// Partial specialization - regular function r-references l-references
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&&)(_Args...)&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&&)(_Args..., ...)&> { using type = _R; };

	// Partial specialization - regular function r-references r-references
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&&)(_Args...)&&> { using type = _R; };
	template <typename _R, typename... _Args>
	struct _return_type_impl<_R(&&)(_Args..., ...)&&> { using type = _R; };

	// Partial specialization - member functions
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...)> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...)> { using type = _R; };

	// Partial specialization - member functions l-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...)&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...)&> { using type = _R; };

	// Partial specialization - member functions r-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...)&&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...)&&> { using type = _R; };

	// Partial specialization - const member functions
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) const> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) const> { using type = _R; };

	// Partial specialization - const member functions l-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) const&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) const&> { using type = _R; };

	// Partial specialization - const member functions r-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) const&&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) const&&> { using type = _R; };

	// Partial specialization - volatile member functions
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) volatile> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) volatile> { using type = _R; };

	// Partial specialization - volatile member functions l-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) volatile&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) volatile&> { using type = _R; };

	// Partial specialization - volatile member functions r-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) volatile&&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) volatile&&> { using type = _R; };

	// Partial specialization - const volatile member functions
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) const volatile> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) const volatile> { using type = _R; };

	// Partial specialization - const volatile member functions l-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) const volatile&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) const volatile&> { using type = _R; };

	// Partial specialization - const volatile member functions r-references
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args...) const volatile&&> { using type = _R; };
	template <typename _R, typename C, typename... _Args>
	struct _return_type_impl<_R(C::*)(_Args..., ...) const volatile&&> { using type = _R; };

	// Main template
	template <typename _T, typename = void>
	struct return_type
		: _return_type_impl<_T> {};

	// Functor specialization
	template <typename _T>
	struct return_type<_T, decltype(void(&_T::operator()))>
		: _return_type_impl<decltype(&_T::operator())> {};

	// return_type helper
	template <typename _T>
	using return_type_t = typename return_type<_T>::type;

	// Function lazy evaluation wrapper
	/// <summary> A lazy evaluate wrapper. </summary>
	/// <remarks> Blue Wing, 2020/3/13. </remarks>
	/// <typeparam name="_F">    Type of the function. </typeparam>
	/// <typeparam name="_Args"> Type of the arguments. </typeparam>
	template<typename _F, typename... _Args>
	class lazy_evaluate {
	public:
		/// <summary> Constructor. </summary>
		/// <remarks> Blue Wing, 2020/3/13. </remarks>
		/// <param name="fn">	   [in] The function. </param>
		/// <param name="...args"> [in] The argument passed in. </param>
		_CONSTEXPR_FN lazy_evaluate(_F&& fn, _Args&& ...args) noexcept
			: fn_(std::bind(std::forward<_F>(fn), std::forward<_Args>(args)...)) {}

		/// <summary> Default constructor(disabled). </summary>
		/// <remarks> Blue Wing, 2020/3/13. </remarks>
		_CONSTEXPR_FN lazy_evaluate() noexcept = delete;

	public:
		/// <summary> Gets the value. </summary>
		/// <remarks> Blue Wing, 2020/3/13. </remarks>
		/// <returns> Value of the return type of the passed function </returns>
		_CONSTEXPR_FN return_type_t<_F> value() noexcept {
			if (value_)
				return value_.value();
			value_.emplace(std::move(fn_()));
			return value_.value();
		}

		/// <summary> Resets the stored value. </summary>
		/// <remarks> Blue Wing, 2020/3/13. </remarks>
		_CONSTEXPR_FN void reset() noexcept {
			value_.reset();
		}

	private:
		std::function<return_type_t<_F>()>  fn_;		// Real running function
#ifdef _OPTIONAL_	// Define in <optional>
		std::optional<return_type_t<_F>> value_;		// Optional value
#else
		boost::optional<return_type_t<F>> value_;		// Optional value
#endif
	};
#endif // !NOT_SUPPORT_LAZY_EVALUATION

	template<typename _T>
	struct _scalar_wrapper;

	template<typename _T>
	struct _non_scalar_support {};

	/// <summary> A macro that defines modern (above C++11) unwrap binary operation. </summary>
	/// <remarks> Blue Wing, 2020/3/14. </remarks>
	/// <param name="_operator"> The operator. </param>
#define BINARY_OP_UNWRAP_MODERN(_operator)										\
public: template<typename _Result>												\
_CONSTEXPR_FN typename _Result unwrap() const _NOEXCEPT {						\
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

/// <summary> A macro that defines combine implementation with binary operation. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_impl"> The implementation name. </param>
/// <param name="_operator">  The operator. </param>
#ifdef _TYPE_TRAITS_
#define COMBINE_IMPL_WITH_BINARY_OP(_impl, _operator, _scalar)							\
template<typename _Lhs, typename _Rhs, typename _Element,								\
		 typename std::enable_if<std::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>						\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,								\
				const _Element& rhs) _NOEXCEPT {										\
	return _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>(lhs, rhs);				\
}																						\
template<typename _Lhs, typename _Rhs, typename _Element,								\
		 typename std::enable_if<!std::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _Element>									\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,								\
				const _Element& rhs) _NOEXCEPT {										\
	return _impl##<##_impl##<_Lhs, _Rhs>, _Element>(lhs, rhs);							\
}																						\
template<typename _Lhs, typename _Rhs, typename _Element,								\
		 typename std::enable_if<std::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>						\
	operator _operator##(const _Element& lhs,											\
				const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {								\
	return _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>(lhs, rhs);				\
}																						\
template<typename _Lhs, typename _Rhs, typename _Element,								\
		 typename std::enable_if<!std::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<_Element, _impl##<_Lhs, _Rhs>>									\
	operator _operator##(const _Element& lhs,											\
				const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {								\
	return _impl##<_Element, _impl##<_Lhs, _Rhs>>(lhs, rhs);							\
}																						\
template<typename _Lhs, typename _Rhs,													\
		 typename std::enable_if<std::is_arithmetic<_Lhs>::value						\
							 && !std::is_arithmetic<_Rhs>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<##_scalar##<_Lhs>, _Rhs>											\
	operator _operator##(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {					\
	return _impl##<##_scalar##<_Lhs>, _Rhs>(lhs, rhs);									\
}																						\
template<typename _Lhs, typename _Rhs,													\
		 typename std::enable_if<std::is_arithmetic<_Rhs>::value						\
							 && !std::is_arithmetic<_Lhs>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<_Lhs, _scalar##<_Rhs>>											\
	operator _operator##(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {					\
	return _impl##<_Lhs, _scalar##<_Rhs>>(lhs, rhs);									\
}																						\
template<typename _Lhs, typename _Rhs,													\
		 typename std::enable_if<!std::is_arithmetic<_Rhs>::value						\
							  && !std::is_arithmetic<_Lhs>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<_Lhs, _Rhs>														\
	operator _operator##(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {					\
	return _impl##<_Lhs, _Rhs>(lhs, rhs);												\
}
#else
#define COMBINE_IMPL_WITH_BINARY_OP(_impl, _operator, _scalar)								\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<boost::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>							\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,									\
				const _Element& rhs) _NOEXCEPT {											\
	return _impl##<##_impl##<_Lhs, _Rhs>, _scalar##<_Element>>(lhs, rhs);					\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<!boost::is_arithmetic<_Element>::value, int>::type = 0>\
_CONSTEXPR_FN _impl##<##_impl##<_Lhs, _Rhs>, _Element>										\
	operator _operator##(const _impl##<_Lhs, _Rhs>& lhs,									\
				const _Element& rhs) _NOEXCEPT {											\
	return _impl##<##_impl##<_Lhs, _Rhs>, _Element>(lhs, rhs);								\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<boost::is_arithmetic<_Element>::value, int>::type = 0>	\
_CONSTEXPR_FN _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>							\
	operator _operator##(const _Element& lhs,												\
				const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {									\
	return _impl##<##_scalar##<_Element>, _impl##<_Lhs, _Rhs>>(lhs, rhs);					\
}																							\
template<typename _Lhs, typename _Rhs, typename _Element,									\
		 typename boost::enable_if_c<!boost::is_arithmetic<_Element>::value, int>::type = 0>\
_CONSTEXPR_FN _impl##<_Element, _impl##<_Lhs, _Rhs>>										\
	operator _operator##(const _Element& lhs,												\
				const _impl##<_Lhs, _Rhs>& rhs) _NOEXCEPT {									\
	return _impl##<_Element, _impl##<_Lhs, _Rhs>>(lhs, rhs);								\
}																							\
template<typename _Lhs, typename _Rhs,														\
		 typename boost::enable_if_c<boost::is_arithmetic<_Lhs>::value						\
							 && !boost::is_arithmetic<_Rhs>::value, int>::type = 0>			\
_CONSTEXPR_FN _impl##<##_scalar##<_Lhs>, _Rhs>												\
	operator _operator##(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {						\
	return _impl##<##_scalar##<_Lhs>, _Rhs>(lhs, rhs);										\
}																							\
template<typename _Lhs, typename _Rhs,														\
		 typename boost::enable_if_c<boost::is_arithmetic<_Rhs>::value						\
							 && !boost::is_arithmetic<_Lhs>::value, int>::type = 0>			\
_CONSTEXPR_FN _impl##<_Lhs, _scalar##<_Rhs>>												\
	operator _operator##(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {						\
	return _impl##<_Lhs, _scalar##<_Rhs>>(lhs, rhs);										\
}																							\
template<typename _Lhs, typename _Rhs,														\
		 typename boost::enable_if_c<!boost::is_arithmetic<_Rhs>::value						\
							  && !boost::is_arithmetic<_Lhs>::value, int>::type = 0>		\
_CONSTEXPR_FN _impl##<_Lhs, _Rhs>															\
	operator _operator##(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {						\
	return _impl##<_Lhs, _Rhs>(lhs, rhs);													\
}
#endif // _TYPE_TRAITS_

/// <summary> A macro that defines create and combine binary operation. </summary>
/// <remarks> Blue Wing, 2020/3/15. </remarks>
/// <param name="_impl">	 The implementation name. </param>
/// <param name="_operator"> The operator. </param>
#define CREATE_AND_COMBINE_BINARY_OP(_impl, _operator, _scalar)					\
CREATE_BINARY_OP_IMPL(_impl, _operator)											\
COMBINE_IMPL_WITH_BINARY_OP(_impl, _operator, _scalar)

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
template<typename _Un = _return_type##>											\
_CONSTEXPR_FN _Un unwrap() const _NOEXCEPT {									\
	return _variable_name##;													\
}

template<typename _T>
struct _scalar_wrapper {
	_T value_;

	_CONSTEXPR_FN _scalar_wrapper(const _T& value) _NOEXCEPT
		: value_(value) {}
	CLS_UNUSED_CONSTRUCTOR(_scalar_wrapper)
	CLS_UNWRAP(_scalar_wrapper, _T, value_)
	_CONSTEXPR_FN operator _T() const _NOEXCEPT {
		return value_;
	}
};

// Support operator + - / with Matrix and scalar
CREATE_AND_COMBINE_BINARY_OP(add_op_impl, +, _non_scalar_support)
CREATE_AND_COMBINE_BINARY_OP(sub_op_impl, -, _non_scalar_support)
CREATE_AND_COMBINE_BINARY_OP(mul_op_impl, *, _scalar_wrapper)

	/// <summary> A wrapper class for Eigen to support. </summary>
	/// <remarks> Blue Wing, 2020/3/14. </remarks>
	/// <typeparam name="_T"> Type of the t. </typeparam>
	template<typename _T>
	class Matrix {
	public:
		typedef Eigen::Matrix<_T, Eigen::Dynamic, Eigen::Dynamic> base_type;
		CLS_BINARY_OP(Matrix, base_type, value_, add_op_impl)
		CLS_BINARY_OP(Matrix, base_type, value_, sub_op_impl)
		CLS_BINARY_OP(Matrix, base_type, value_, mul_op_impl)
		CLS_UNWRAP(Matrix, base_type, value_)

	public:
		_CONSTEXPR_FN Matrix() {}
		_CONSTEXPR_FN Matrix(const size_t row_size, const size_t col_size) {}
		_CONSTEXPR_FN Matrix(const size_t row_size, const size_t nCols, _T default_values[]) {}
		_CONSTEXPR_FN Matrix(const size_t square_size) {}
		_CONSTEXPR_FN Matrix(const size_t square_size, _T default_values[]) {}
		_CONSTEXPR_FN ~Matrix() {}

	public:
		_CONSTEXPR_FN bool Init(const size_t row_size, const size_t col_size) {}
		_CONSTEXPR_FN bool MakeUnitMatrix(const size_t square_size) {}
		std::string ToString(const std::string& delim = " ", const bool line_break = true) {}

	public:
		_CONSTEXPR_FN bool SetElement(const size_t row, const size_t col, _T value) {}
		_CONSTEXPR_FN _T GetElement(const size_t row, const size_t col) const {}
		_CONSTEXPR_FN void SetData(_T value[]) {}
		_CONSTEXPR_FN size_t GetNumColumns() const {}
		_CONSTEXPR_FN size_t GetNumRows() const {}
		_CONSTEXPR_FN size_t GetRowVector(const size_t row, _T* row_element_list) const {}
		_CONSTEXPR_FN size_t GetColVector(const size_t col, _T* row_element_list) const {}
		_CONSTEXPR_FN _T* GetData() const {}
		_CONSTEXPR_FN _T Max(size_t& row, size_t& col) {}
		_CONSTEXPR_FN _T Max() const {}
		_CONSTEXPR_FN _T Min(size_t& nRow, size_t& nCol) {}
		_CONSTEXPR_FN _T Min() const {}

	public:
		_CONSTEXPR_FN Matrix<_T> Transpose() const {}
		_CONSTEXPR_FN Matrix<_T> Abs() const {}
		_CONSTEXPR_FN Matrix<_T> Inv() const {}
		_CONSTEXPR_FN Matrix<_T> Inv_Ssgj() const {}
		_CONSTEXPR_FN Matrix<_T> InvertTriAngle() const {}
		_CONSTEXPR_FN _T DetGauss() const {}
		_CONSTEXPR_FN size_t RankGauss() const {}

	private:
		base_type value_;	// The matrix value
	};
}


#endif	// #ifndef _NUDTTK_MATH_MATRIX_TR_