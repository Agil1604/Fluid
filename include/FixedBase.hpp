#ifndef FIXEDBASE_
#define FIXEDBASE_

#include "Const.hpp"
#include<bits/stdc++.h>

template <typename V, size_t K>
requires (sizeof(V) * CHAR_BIT >= K)
class FixedBase {
public:
    template <typename V1, size_t K1>
    constexpr FixedBase(FixedBase<V1, K1> f): v(K > K1 ? f.v << (K-K1) : f.v >> (K1-K)) {}
    
    constexpr FixedBase(int v): v(v << K) {}
    constexpr FixedBase(float f): v(f * (1 << K)) {}
    constexpr FixedBase(double f): v(f * (1 << K)) {}
    constexpr FixedBase(): v(0) {}

    static constexpr FixedBase from_raw(V x) {
        FixedBase ret;
        ret.v = x;
        return ret;
    } 

    V v;

    auto operator<=>(const FixedBase&) const = default;
    bool operator==(const FixedBase&) const = default;

    explicit constexpr operator float() const {return (double)v / ((double) (1 << K));}
    explicit constexpr operator double() const {return (double)v / ((double) (1 << K));}

    template <typename V1, size_t K1>
    FixedBase &operator=(const FixedBase<V1, K1> b);
};

template <typename V1, size_t K1, typename V2, size_t K2>
auto operator<=>(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b){
    if (K1 >= K2){
        return a.v <=> (b.v << (K1-K2));
    } else {
        return a.v <=> (b.v >> (K2-K1));
    }
}

template <typename V1, size_t K1, typename V2, size_t K2>
bool operator==(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b){
    if (K1 >= K2){
        return a.v == (b.v << (K1-K2));
    } else {
        return a.v == (b.v >> (K2-K1));
    }
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> operator+(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b) {
    if (K1 >= K2){
        return FixedBase<V1, K1>::from_raw(a.v + (b.v << (K1-K2)));
    } else {
        return FixedBase<V1, K1>::from_raw(a.v + (b.v >> (K2-K1)));
    }
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> operator-(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b) {
    if (K1 >= K2){
        return FixedBase<V1, K1>::from_raw(a.v - (b.v << (K1-K2)));
    } else {
        return FixedBase<V1, K1>::from_raw(a.v - (b.v >> (K2-K1)));
    }}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> operator*(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b) {
    return FixedBase<V1, K1>::from_raw(((int64_t) a.v * b.v) >> K2);
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> operator/(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b) {
    return FixedBase<V1, K1>::from_raw(((int64_t) a.v << K2) / b.v);
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> &operator+=(FixedBase<V1, K1>& a, const FixedBase<V2, K2> b) {
    return a = a + b;
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> &operator-=(FixedBase<V1, K1>& a, const FixedBase<V2, K2> b) {
    return a = a - b;
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> &operator*=(FixedBase<V1, K1>& a, const FixedBase<V2, K2> b) {
    return a = a * b;
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> &operator/=(FixedBase<V1, K1>& a, const FixedBase<V2, K2> b) {
    return a = a / b;
}

template <typename V, size_t K>
FixedBase<V, K> operator-(const FixedBase<V, K> x) {
    return FixedBase<V, K>::from_raw(-x.v);
}

template <typename V, size_t K>
FixedBase<V, K> abs(const FixedBase<V, K> x) {
    if (x.v < 0) {
        x.v = -x.v;
    }
    return x;
}

template <typename V, size_t K>
std::istream &operator>>(std::istream &in, FixedBase<V, K>& x) {
    double tmp;
    in >> tmp;
    x = FixedBase<V, K>{tmp};
    return in;
}

template <typename V, size_t K>
std::ostream &operator<<(std::ostream &out, FixedBase<V, K> x) {
    return out << x.v / (double) (1ll << K);
}

template <typename V, size_t K>
template <typename V1, size_t K1>
FixedBase<V, K> &FixedBase<V, K>::operator=(const FixedBase<V1, K1> b) {
    if (*this == b)
        return *this;
    
    this->v = b.v;
    return *this;
}

template <typename V1, size_t K1, typename V2, size_t K2>
FixedBase<V1, K1> min(const FixedBase<V1, K1> a, const FixedBase<V2, K2> b) {
    return a <= b ? a : FixedBase<V1, K1>(b);
}

template <typename V1, size_t K1, typename V2>
FixedBase<V1, K1> min(const FixedBase<V1, K1> a, const V2 b) {
    return a <= b ? a : FixedBase<V1, K1>(b);
}

template <typename V1, typename V2>
V1 min(const V1 a, const V2 b) {
    return std::min(a, b);
}

#endif