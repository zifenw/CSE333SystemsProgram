/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/14/25
 */
#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <string>
using std::string;

class Coordinate {
 public:
    // Pure virtual function
    virtual string ToString() const = 0;
};

#endif  // COORDINATE_H_
