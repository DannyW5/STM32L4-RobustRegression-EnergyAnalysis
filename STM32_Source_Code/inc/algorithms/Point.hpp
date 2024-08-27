#ifndef POINT_H
#define POINT_H

#include <utility>

template <typename T>
struct Point{
    T x;
    T y;

    bool operator<(const Point& other) const {
        return x < other.x; 
    }
};

#endif