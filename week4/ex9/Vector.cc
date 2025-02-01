/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/27/31
 */
#include <iostream>
#include "Vector.h"

namespace vector333 {

// Default Constructor
Vector::Vector() : x_(0.0), y_(0.0), z_(0.0) {}

// Parameterized Constructor
Vector::Vector(const double x, const double y, const double z)
    : x_(x), y_(y), z_(z) {}

// Copy Constructor
Vector::Vector(const Vector& other)
    : x_(other.x_), y_(other.y_), z_(other.z_) {}

// Destructor
Vector::~Vector() {}

// Accessors
double Vector::get_x() const { return x_; }
double Vector::get_y() const { return y_; }
double Vector::get_z() const { return z_; }

// Assignment Operator
Vector &Vector::operator=(const Vector &other) {
    if (this != &other) {
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
    }
    return *this;
}

// Update Assignment Operators
Vector &Vector::operator+=(const Vector& other) {
    x_ += other.x_;
    y_ += other.y_;
    z_ += other.z_;
    return *this;
}

Vector &Vector::operator-=(const Vector& other) {
    x_ -= other.x_;
    y_ -= other.y_;
    z_ -= other.z_;
    return *this;
}

// Arithmetic Operators
Vector Vector::operator+(const Vector& other) {
    return Vector(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Vector Vector::operator-(const Vector& other) {
    return Vector(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

double Vector::operator*(const Vector &other) {
  return x_*other.x_ + y_*other.y_ + z_*other.z_;
}

Vector Vector::operator*(const double k) {
  return Vector(x_ * k, y_ * k, z_ * k);
}

std::ostream & operator<<(std::ostream &os, const Vector &v) {
    os << "(" << v.x_ << "," << v.y_ << "," << v.z_ << ")";
    return os;
}
}  // namespace vector333
