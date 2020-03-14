#pragma once

#ifndef _NUDTTK_MATH_COMMON_DEFINITION_TR_
#define _NUDTTK_MATH_COMMON_DEFINITION_TR_

// constexpr usage
#ifndef _CONSTEXPR
#if __cplusplus < 201103L
#define _CONSTEXPR			const
#define _CONSTEXPR_FN
#define _CONSTEXPR_FN_A
#define _NOEXCEPT
#else	// __cplusplus >= 201103L
#define _CONSTEXPR			constexpr
#if __cplusplus > 201103L
#define _CONSTEXPR_FN		constexpr
#else	// __cplusplus == 201103L
#define _CONSTEXPR_FN
#endif	// __cplusplus > 201103L
#define _CONSTEXPR_FN_A		constexpr
#define _NOEXCEPT			noexcept
#endif	// __cplusplus < 201103L

#endif // !_CONSTEXPR

#endif // !_NUDTTK_MATH_COMMON_DEFINITION_TR_