/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/14/25
 */
#include <cmath>
#include <string>
#include <sstream>
#include "Cartesian.h"

using std::string;
using std::to_string;

// Constructor: Initializes Cartesian coordinates with given values
Cartesian::Cartesian(double x, double y) : x_(x), y_(y) {}

// Converts Cartesian coordinates to a string representation "(x, y)"
string Cartesian::ToString() const {
    std::stringstream ss;
    ss << "(" << x_ << ", " << y_ << ")";
    return ss.str();
}

// Calculates the Euclidean distance between this Cartesian point and another
double Cartesian::Distance(const Cartesian& other) const {
  return sqrt(pow(x_ - other.x_, 2.0) + pow(y_ - other.y_, 2.0));
}
