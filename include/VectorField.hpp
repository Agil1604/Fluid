#ifndef VECTORFIELD_
#define VECTORFIELD_

#include "Fixed.hpp"
template <typename T, int Nv, int Mv>
class VectorField {
public:
    std::array<T, deltas.size()> v[Nv][Mv];

    T &add(int x, int y, int dx, int dy, T dv);
    T &get(int x, int y, int dx, int dy);
};

template <typename T, int Nv, int Mv>
T& VectorField<T, Nv, Mv>::get(int x, int y, int dx, int dy) {
    switch (2*dx + dy){
    case 1:
        return v[x][y][3];
    case 2:
        return v[x][y][1];
    case -1:
        return v[x][y][2];
    case -2:
        return v[x][y][0];
    default:
        exit(-1);
    }
}

template <typename T, int Nv, int Mv>
T & VectorField<T, Nv, Mv>::add(int x, int y, int dx, int dy, T dv) {
    return get(x, y, dx, dy) += dv;
}

#endif