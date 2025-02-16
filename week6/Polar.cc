/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/14/25
 */
#include "Polar.h"
#include <cmath>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;

// Constructor to initialize the Polar point (r, a)
Polar::Polar(double r, double a) : r_(r), a_(a) {}

// Overridden function to return string representation of coordinates
string Polar::ToString() const {
    std::stringstream ss;
    ss << "(" << r_ << ", " << a_ << ")";
    return ss.str();
}

// Method to calculate the Euclidean distance between two Polar points
// Uses the law of cosines formula for polar coordinates
// angle is measured in radians here
double Polar::Distance(const Polar& other) const {
    return sqrt(r_ * r_ + other.r_ * other.r_ -
                2 * r_ * other.r_ * std::cos(a_ - other.a_));
}
