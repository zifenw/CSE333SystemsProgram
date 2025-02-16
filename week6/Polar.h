/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/14/25
 */
#ifndef POLAR_H_
#define POLAR_H_

#include <string>
#include "Coordinate.h"

using std::string;

class Polar : public Coordinate {
 public:
    // Constructor to initialize the Polar point (r, θ)
    Polar(double r, double a);
    // Overridden function to return string representation of coordinates
    string ToString() const override;
    // Method to calculate the Euclidean distance between two Polar points
    double Distance(const Polar& other) const;
 private:
    double r_, a_;  // polar coordinates radius and angle
};

#endif  // POLAR_H_
