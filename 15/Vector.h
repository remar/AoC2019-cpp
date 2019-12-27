// -*- mode: c++ -*-

#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector {
public:
  float x, y;

  Vector() : x(0), y(0) {}

  void set(float x, float y) {
    this->x = x;
    this->y = y;
  }

  void normalize() {
    float len = std::sqrt(x * x + y * y);

    if(len == 0) {
      return;
    }

    x /= len;
    y /= len;
  }
};

#endif
