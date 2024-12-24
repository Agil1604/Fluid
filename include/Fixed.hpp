#ifndef FIXED_
#define FIXED_

#include "FixedBase.hpp"

// Fixed
template <size_t N>
struct SizeToType;

template<> struct SizeToType<8>  {using type = int8_t; };
template<> struct SizeToType<16> {using type = int16_t;};
template<> struct SizeToType<32> {using type = int32_t;};
template<> struct SizeToType<64> {using type = int64_t;};

template <size_t N, size_t K>
requires (N <= 64 && N >= K)
using Fixed = FixedBase<typename SizeToType<N>::type, K>;




// FastFixed

template <size_t N>
struct FastSizeToType;

template<> struct FastSizeToType<8> {using type = int_fast8_t;};
template<> struct FastSizeToType<16> {using type = int_fast16_t;};
template<> struct FastSizeToType<32> {using type = int_fast32_t;};
template<> struct FastSizeToType<64> {using type = int_fast64_t;};

template <size_t N>
struct FastSizeToType{using type = typename FastSizeToType<N+1>::type;};

template <size_t N, size_t K>
requires (N <= 64 && N >= K)
using FastFixed = FixedBase<typename FastSizeToType<N>::type, K>;

#endif