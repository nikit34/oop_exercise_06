#pragma once
#include <array>
#include "vertex.h"


template<typename T>
double get_diagonal(const vertex_t<T> p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4) {
    T d1 = vertex::lenght(p1, p2);
    T d2 = vertex::lenght(p1, p3);
    T d3 = vertex::lenght(p1, p4);
    if(d1 == d2) {
        return d3;
    } else if(d1 == d3) {
        return d2;
    } else if(d2 == d3) {
        return d1;
    } else {
        throw std::invalid_argument("Entered coordinates are not forming Rhombus. Try entering new coordinates");
    }
}

template<typename T>
struct Rhombus {
    Rhombus(const vertex_t<T>& p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4);

    double area() const;
    vertex_t<T> center() const;
    void print(std::ostream& os) const;

    private:
        std::array<vertex_t<T>, 4> points;
        T small_diagonal, big_diagonal;
};

template<typename T>
Rhombus<T>::Rhombus(const vertex_t<T>& p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4) {
    try {
        T d1 = get_diagonal(p1, p2, p3, p4);
        T d2 = get_diagonal(p2, p1, p3, p4);
        T d3 = get_diagonal(p3, p1, p2, p4);
        T d4 = get_diagonal(p4, p1, p2, p3);
        if(d1 == d2 || d1 == d4) {
            if(d1 < d3) {
                this->small_diagonal = d1;
                this->big_diagonal = d3;
            } else {
                this->small_diagonal = d3;
                this->big_diagonal = d1;
            }
        } else if(d1 == d3) {
            if(d1 < d2) {
                this->small_diagonal = d1;
                this->big_diagonal = d2;
            } else {
                this->small_diagonal = d2;
                this->big_diagonal = d1;
            }
        }
    } catch(std::exception& e) {
        throw std::invalid_argument(e.what());
        return;
    }
    this->points[0] = p1;
    this->points[1] = p2;
    this->points[2] = p3;
    this->points[3] = p4;
}

template<typename T>
double Rhombus<T>::area() const {
    return this->small_diagonal * this->big_diagonal / 2.0;
}

template<typename T>
vertex_t<T> Rhombus<T>::center() const {
    if (vertex::length(this->points[0], this->points[1]) == this->small_diagonal ||
    vertex::length(this->points[0], this->points[1]) == this->big_diagonal) {
        return {
            ((this->points[0].x + this->points[1].x) / 2.0),
            ((this->points[0].y + this->points[1].y) / 2.0)
        };
    } else if (vertex::length(this->points[0], this->points[2]) == this->small_diagonal ||
    vertex::length(this->points[0], this->points[2]) == this->big_diagonal) {
        return {
            ((this->points[0].x + this->points[2].x) / 2.0),
            ((this->points[0].y + this->points[2].y) / 2.0)
        };
    } else {
        return {
            ((this->points[0].x + this->points[3].x) / 2.0),
            ((this->points[0].y + this->points[3].y) / 2.0)
        };
    }
}

template<typename T>
void Rhombus<T>::print(std::ostream& os) const {
    os << "Rhombus: ";
    for (const auto& p : this->points) {
        os << p << ' ';
    }
    os << std::endl;
}