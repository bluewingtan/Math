// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <boost/utility.hpp>
#include <boost/type_traits.hpp>

#include "common.h"

template<typename _T>
struct _scalar_wrapper;

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
_CONSTEXPR_FN _impl##() _NOEXCEPT = delete;										\

/// <summary> A macro that defines legacy (below C++11) binary operation unused constructor. </summary>
/// <remarks> Blue Wing, 2020/3/14. </remarks>
/// <param name="_impl"> The implement. </param>
#define BINARY_OP_UNUSED_CONSTRUCTOR_LEGACY(_impl)								\
private: _impl##(){};															\

// Check support status
#ifdef NOT_SUPPORT_LAZY_EVALUATION
#define BINARY_OP_UNWRAP BINARY_OP_UNWRAP_LEGACY
#define BINARY_OP_UNUSED_CONSTRUCTOR BINARY_OP_UNUSED_CONSTRUCTOR_LEGACY
#else 
#define BINARY_OP_UNWRAP BINARY_OP_UNWRAP_MODERN
#define BINARY_OP_UNUSED_CONSTRUCTOR BINARY_OP_UNUSED_CONSTRUCTOR_MODERN
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

CREATE_AND_COMBINE_BINARY_OP(add_op_impl, +, _scalar_wrapper)
CREATE_AND_COMBINE_BINARY_OP(sub_op_impl, -, _scalar_wrapper)
CREATE_AND_COMBINE_BINARY_OP(mul_op_impl, *, _scalar_wrapper)
CREATE_AND_COMBINE_BINARY_OP(div_op_impl, /, _scalar_wrapper)

template<typename _T>
struct S {
	_T value_;

	S(_T value = static_cast<_T>(0)) :value_(value) {}

	CLS_BINARY_OP(S, _T, value_, add_op_impl)
	CLS_BINARY_OP(S, _T, value_, sub_op_impl)
	CLS_BINARY_OP(S, _T, value_, mul_op_impl)
	CLS_BINARY_OP(S, _T, value_, div_op_impl)

	CLS_UNWRAP(S, _T, value_)

	constexpr operator _T() const {
		return value_;
	}
};

template<typename _T>
struct _scalar_wrapper {
	_T value_;

	_CONSTEXPR_FN _scalar_wrapper(const _T& value) _NOEXCEPT
		: value_(value) {}
	BINARY_OP_UNUSED_CONSTRUCTOR(_scalar_wrapper)
	CLS_UNWRAP(_scalar_wrapper, _T, value_)
	constexpr operator _T() const {
		return value_;
	}
};

template<typename _T, typename boost::enable_if_c<boost::is_arithmetic<_T>::value, int>::type = 0>
_scalar_wrapper<_T> test(const _T& value) {
	return _scalar_wrapper(value);
}

template<typename _T, typename boost::enable_if_c<!boost::is_arithmetic<_T>::value, int>::type = 0>
_T test(const _T& value) {
	return value;
}

int main() {
	S a(1), c(3);
	S<int> b = a * 3 * c + a;
	std::cout << b.unwrap();
	return 0;
}