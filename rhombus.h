#pragma once
#include <array>
#include "vertex.h"


template<typename T>
double Rhombus<T>::get_diagonal(int& arr_index) {
    T d1 = vertex::lenght(this->points[arr_index[0]], this->points[arr_index[1]]);
    T d2 = vertex::lenght(this->points[arr_index[0]], this->points[arr_index[2]]);
    T d3 = vertex::lenght(this->points[arr_index[0]], this->points[arr_index[3]]);
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
        T d1 = get_diagonal({1, 2, 3, 4});
        T d2 = get_diagonal({2, 1, 3, 4});
        T d3 = get_diagonal({3, 1, 2, 4});
        T d4 = get_diagonal({4, 1, 2, 3});
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
struct Rhombus {
    Rhombus() {};
    Rhombus(const vertex_t<T>& p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4);

    double area() const;
    vertex_t<T> center() const;
    void print(std::ostream& os) const;

    void set_diagonal();
    double get_diagonal(int& arr_index);
    std::istream& set_coord(std::istream& is);

    private:
        std::array<vertex_t<T>, 4> points;
        T small_diagonal, big_diagonal;
};

template<typename T>
Rhombus<T>::Rhombus(const vertex_t<T>& p1, const vertex_t<T>& p2, const vertex_t<T>& p3, const vertex_t<T>& p4) {
    this->points[0] = p1;
    this->points[1] = p2;
    this->points[2] = p3;
    this->points[3] = p4;
    this->set_diagonal();
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

template<typename T>
std::istream &Rhombus<T>::set_coord(std::istream& is) {
	for (int i = 0; i < 4; i++) {
		is >> this->points[i];
	}
    this->set_diagonal();
	return is;
}

template<typename T>
std::istream &operator>> (std::istream &is, Rhombus<T> &t) {
	return set_coord(is, t);
}