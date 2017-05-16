#ifndef POINT_HPP
#define POINT_HPP

#include <stdint.h>

typedef int32_t frame_id;

typedef enum {
    UpperLeft,
    LowerLeft,
    UpperRight,
    LowerRight
} Corners;

typedef struct Point {
    int32_t x;
    int32_t y;
    Point() : x(0), y(0) {};    
    Point(int32_t x, int32_t y) : x(x), y(y) {};
    Point(int32_t value) : x(value), y(value) {};
    // Point &operator=(Point &p) {this->x = p.x; this->y = p.y; return *this;}
} Point;

#endif