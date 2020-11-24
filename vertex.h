#pragma once

#include <iostream>
#include <cmath>
#include <iomanip>


template<typename T>
struct vertex_t {
    T x;
    T y;
};

template<typename T>
std::istream& operator>> (std::istream& is, vertex_t<T>& p) {
    is >> p.x >> p.y;
    return is;
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const vertex_t<T>& p) {
    os  << std::fixed << std::setprecision(3) << "[" << p.x << ", " << p.y << "]";
    return os;
}

namespace vertex {
    template<typename T>
    T length(const vertex_t<T>& p1, const vertex_t<T>& p2) {
        return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
    }
}