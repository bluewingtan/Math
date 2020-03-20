// Math.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "matrix.h"

template<typename _T>
struct _scalar_support {
	_T value_;

	_CONSTEXPR_FN _scalar_support(const _T& value) _NOEXCEPT
		: value_(value) {}

	_scalar_support() = delete;

	template<typename _U = _T>
	_CONSTEXPR_FN _U unwrap() const _NOEXCEPT {
		return value_;
	}

	_CONSTEXPR_FN operator _T() const _NOEXCEPT {
		return value_;
	}
};

template<typename _Lhs, typename _Rhs>
class mul_op_impl {
public:
	_CONSTEXPR_FN mul_op_impl(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT
		: lhs_(lhs), rhs_(rhs) {}

	_CONSTEXPR_FN mul_op_impl() = delete;

	template<typename _Result>
	_CONSTEXPR_FN _Result unwrap() const _NOEXCEPT {
		return lhs_.unwrap<_Result>() * rhs_.unwrap<_Result>();
	}
private:
	const _Lhs& lhs_;
	const _Rhs& rhs_;
};

template<typename _Lhs, typename _Rhs, typename _Element,
	typename std::enable_if<std::is_arithmetic<_Element>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<mul_op_impl<_Lhs, _Rhs>, _scalar_support<_Element>>
	operator *(const mul_op_impl<_Lhs, _Rhs>& lhs,
			   const _Element& rhs) _NOEXCEPT {
	return mul_op_impl<mul_op_impl<_Lhs, _Rhs>, _scalar_support<_Element>>(lhs, rhs);
}
template<typename _Lhs, typename _Rhs, typename _Element,
	typename std::enable_if<!std::is_arithmetic<_Element>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<mul_op_impl<_Lhs, _Rhs>, _Element>
	operator *(const mul_op_impl<_Lhs, _Rhs>& lhs,
			   const _Element& rhs) _NOEXCEPT {
	return mul_op_impl<mul_op_impl<_Lhs, _Rhs>, _Element>(lhs, rhs);
}
template<typename _Lhs, typename _Rhs, typename _Element,
	typename std::enable_if<std::is_arithmetic<_Element>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<_scalar_support<_Element>, mul_op_impl<_Lhs, _Rhs>>
	operator *(const _Element& lhs,
			   const mul_op_impl<_Lhs, _Rhs>& rhs) _NOEXCEPT {
	return mul_op_impl<_scalar_support<_Element>, mul_op_impl<_Lhs, _Rhs>>(lhs, rhs);
}
template<typename _Lhs, typename _Rhs, typename _Element,
	typename std::enable_if<!std::is_arithmetic<_Element>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<_Element, mul_op_impl<_Lhs, _Rhs>>
	operator *(const _Element& lhs,
			   const mul_op_impl<_Lhs, _Rhs>& rhs) _NOEXCEPT {
	return mul_op_impl<_Element, mul_op_impl<_Lhs, _Rhs>>(lhs, rhs);
}
template<typename _Lhs, typename _Rhs,
	typename std::enable_if<std::is_arithmetic<_Lhs>::value
	&& !std::is_arithmetic<_Rhs>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<_scalar_support<_Lhs>, _Rhs>
	operator *(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {
	return mul_op_impl<_scalar_support<_Lhs>, _Rhs>(lhs, rhs);
}
template<typename _Lhs, typename _Rhs,
	typename std::enable_if<std::is_arithmetic<_Rhs>::value
	&& !std::is_arithmetic<_Lhs>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<_Lhs, _scalar_support<_Rhs>>
	operator *(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {
	return mul_op_impl<_Lhs, _scalar_support<_Rhs>>(lhs, rhs);
}
template<typename _Lhs, typename _Rhs,
	typename std::enable_if<!std::is_arithmetic<_Rhs>::value
	&& !std::is_arithmetic<_Lhs>::value, int>::type = 0>
	_CONSTEXPR_FN mul_op_impl<_Lhs, _Rhs>
	operator *(const _Lhs& lhs, const _Rhs& rhs) _NOEXCEPT {
	return mul_op_impl<_Lhs, _Rhs>(lhs, rhs);
}

template<typename _T>
class _Test_member_wrapper {
public:
	using base_type = Eigen::Matrix<_T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
public:
	_CONSTEXPR_FN _Test_member_wrapper(const size_t edge_size, _T default_values[])
		: value_(Eigen::Map<base_type>(default_values, edge_size, edge_size)) {}

	~_Test_member_wrapper() {

	}
	template<typename Lhs, typename Rhs>
	_CONSTEXPR_FN _Test_member_wrapper(const mul_op_impl<Lhs, Rhs>& op) _NOEXCEPT {
		value_ = op.unwrap<base_type>();
	}
	template<typename _U = base_type>
	_CONSTEXPR_FN _U unwrap() const _NOEXCEPT {
		return value_;
	}

private:
	base_type value_;
};

int main() {
	double value1[] = { 0.1, 0.2, 0.3, 0.4 };
	double value2[] = { 2, 2, 2, 2 };
	_Test_member_wrapper<double> t1(2, value1), t2(2, value2);
	_Test_member_wrapper<double> t = 3.0 * t2;
	return 0;
}