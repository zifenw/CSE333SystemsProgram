/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/14/25
 */
#ifndef CARTESIAN_H_
#define CARTESIAN_H_

#include <string>
#include "Coordinate.h"

using std::string;

class Cartesian : public Coordinate {
 public:
    // Constructor to initialize the Cartesian point (x, y)
    Cartesian(double x, double y);
    // Overridden function to return string representation of coordinates
    string ToString() const override;
    // Method to calculate the Euclidean distance between two Cartesian points
    double Distance(const Cartesian& other) const;
 private:
    double x_, y_;  // Cartesian coordinates
};

#endif  // CARTESIAN_H_
