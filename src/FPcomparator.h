#pragma once

#include<type_traits>
using std::is_floating_point;

template< class T>
struct FPcomparator final{	// Precise floating point comparator

	static constexpr double epsilon = 0.001;
	const T flt;

	inline constexpr FPcomparator( const T f) noexcept : flt{f} { static_assert( is_floating_point<T>::value,"");}
	inline ~FPcomparator() noexcept = default;

	template< class U> inline constexpr bool operator>=( const U other) const noexcept{ return this->operator==(other) || flt > other; static_assert( is_floating_point<U>::value,"");}
	template< class U> inline constexpr bool operator<=( const U other) const noexcept{ return this->operator==(other) || flt < other; static_assert( is_floating_point<U>::value,"");}

	template< class U> inline constexpr bool operator>( const U other) const noexcept{ return !this->operator==(other) && flt > other; static_assert( is_floating_point<U>::value,"");}
	template< class U> inline constexpr bool operator<( const U other) const noexcept{ return !this->operator==(other) && flt < other; static_assert( is_floating_point<U>::value,"");}

	template< class U> inline constexpr bool operator==( const U other) const noexcept{ return ( flt < other ? other-flt : flt-other ) < epsilon; static_assert( is_floating_point<U>::value,"");}
	template< class U> inline constexpr bool operator!=( const U other) const noexcept{ return !this->operator==(other); static_assert( is_floating_point<U>::value,"");}
};

template< class T, class U> inline constexpr bool operator>=( const T f1, const FPcomparator<U> f2) noexcept{ return f2.operator==(f1) || f1 > f2.flt; static_assert( is_floating_point<T>::value,"");}
template< class T, class U> inline constexpr bool operator<=( const T f1, const FPcomparator<U> f2) noexcept{ return f2.operator==(f1) || f1 < f2.flt; static_assert( is_floating_point<T>::value,"");}

template< class T, class U> inline constexpr bool operator>( const T f1, const FPcomparator<U> f2) noexcept{ return !f2.operator==(f1) && f1 > f2.flt; static_assert( is_floating_point<T>::value,"");}
template< class T, class U> inline constexpr bool operator<( const T f1, const FPcomparator<U> f2) noexcept{ return !f2.operator==(f1) && f1 < f2.flt; static_assert( is_floating_point<T>::value,"");}

template< class T, class U> inline constexpr bool operator==( const T f1, const FPcomparator<U> f2) noexcept{ return f2.operator==(f1); static_assert( is_floating_point<T>::value,"");}
template< class T, class U> inline constexpr bool operator!=( const T f1, const FPcomparator<U> f2) noexcept{ return !f2.operator==(f1); static_assert( is_floating_point<T>::value,"");}
