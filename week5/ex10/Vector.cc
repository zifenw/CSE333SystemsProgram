/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/03/31
 */

#include "Vector.h"

#include <iostream>
using std::ostream;

namespace vector333 {

// Default constructor
Vector::Vector() {
    data_ = new double[3];
    data_[0] = 0.0;  // x
    data_[1] = 0.0;  // y
    data_[2] = 0.0;  // z
}

// Parameterized constructor
Vector::Vector(const double x, const double y, const double z) {
    data_ = new double[3];
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
}

// Copy constructor
Vector::Vector(const Vector &v) {
    data_ = new double[3];
    data_[0] = v.data_[0];
    data_[1] = v.data_[1];
    data_[2] = v.data_[2];
}

// Destructor
Vector::~Vector() {
    delete[] data_;
}

// Assignment operator
Vector &Vector::operator=(const Vector &v) {
    if (this != &v) {
        data_[0] = v.data_[0];
        data_[1] = v.data_[1];
        data_[2] = v.data_[2];
    }
    return *this;
}

// Updating assignment operators
Vector &Vector::operator+=(const Vector &v) {
    data_[0] += v.data_[0];
    data_[1] += v.data_[1];
    data_[2] += v.data_[2];
    return *this;
}

Vector &Vector::operator-=(const Vector &v) {
    data_[0] -= v.data_[0];
    data_[1] -= v.data_[1];
    data_[2] -= v.data_[2];
    return *this;
}

// Friend functions for operators
Vector operator+(const Vector &v1, const Vector &v2) {
    return Vector(v1.data_[0] + v2.data_[0],
                  v1.data_[1] + v2.data_[1],
                  v1.data_[2] + v2.data_[2]);
}

Vector operator-(const Vector &v1, const Vector &v2) {
    return Vector(v1.data_[0] - v2.data_[0],
                  v1.data_[1] - v2.data_[1],
                  v1.data_[2] - v2.data_[2]);
}

double operator*(const Vector &v1, const Vector &v2) {
    return v1.data_[0] * v2.data_[0] +
           v1.data_[1] * v2.data_[1] +
           v1.data_[2] * v2.data_[2];
}

Vector operator*(const Vector &v, const double k) {
    return Vector(v.data_[0] * k,
                  v.data_[1] * k,
                  v.data_[2] * k);
}

Vector operator*(const double k, const Vector &v) {
    return v * k;
}

// Stream output
std::ostream &operator<<(std::ostream &os, const Vector &v) {
    os << "(" << v.data_[0] << "," << v.data_[1] << "," << v.data_[2] << ")";
    return os;
}

}  // namespace vector333