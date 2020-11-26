#pragma once
#include <array>
#include "vertex.h"


template<typename T>
struct Rhombus {
    Rhombus() {};
    Rhombus(const vertex_t<T>& p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4);

    T area() const;
    vertex_t<T> center() const;
    void print(std::ostream& os) const;

    void set_diagonal();
    void set_points(std::pair<T, T> &p, size_t &index);

    template<std::size_t N>
    T get_diagonal(const size_t (&arr_index)[N]);

    private:
        std::array<vertex_t<T>, 4> points;
        T small_diagonal, big_diagonal;
};

template<typename T>
template<std::size_t N>
T Rhombus<T>::get_diagonal(const size_t (&arr_index)[N]) {
    T d1 = vertex::length(this->points[arr_index[0]], this->points[arr_index[1]]);
    T d2 = vertex::length(this->points[arr_index[0]], this->points[arr_index[2]]);
    T d3 = vertex::length(this->points[arr_index[0]], this->points[arr_index[3]]);
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
void Rhombus<T>::set_diagonal(){
    try {
        T d1 = get_diagonal({0, 1, 2, 3});
        T d2 = get_diagonal({1, 0, 2, 3});
        T d3 = get_diagonal({2, 0, 1, 3});
        T d4 = get_diagonal({3, 0, 1, 2});
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
}

template<typename T>
Rhombus<T>::Rhombus(const vertex_t<T>& p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4) {
    this->points[0] = p1;
    this->points[1] = p2;
    this->points[2] = p3;
    this->points[3] = p4;
    this->set_diagonal();
}

template<typename T>
T Rhombus<T>::area() const {
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
    for (const auto& p : this->points)
        os << p << ' ';
    os << std::endl;
}

template<typename T>
void Rhombus<T>::set_points(std::pair<T, T> &p, size_t &index){
    this->points[index].x = p.first;
    this->points[index].y = p.second;
}

template<typename T>
std::istream &operator>> (std::istream &is, Rhombus<T> &item) {
	std::pair<T, T> tmp_input;
    for (size_t i = 0; i < 4; ++i) {
		is >> tmp_input.first >> tmp_input.second;
        item.set_points(tmp_input, i);
    }
    item.set_diagonal();
	return is;
}