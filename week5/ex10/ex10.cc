/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/03/31
 */

#include "Vector.h"
#include <iostream>

using std::cout;
using std::endl;
using vector333::Vector;

int main() {
    // Test constructors
    Vector v1;  // Default constructor
    Vector v2(1.0, -2.0, 3.0);  // Parameterized constructor
    Vector v3(v2);  // Copy constructor

    cout << "v1: " << v1 << endl;
    cout << "v2: " << v2 << endl;
    cout << "v3 (copy of v2): " << v3 << endl;

    // Test assignment operator
    v1 = v2;
    cout << "v1 after assignment: " << v1 << endl;

    // Test updating assignment operators
    v1 += v2;
    cout << "v1 after v1 += v2: " << v1 << endl;

    v1 -= v2;
    cout << "v1 after v1 -= v2: " << v1 << endl;

    // Test arithmetic operators
    Vector v4 = v2 + v3;
    cout << "v2 + v3: " << v4 << endl;

    Vector v5 = v2 - v3;
    cout << "v2 - v3: " << v5 << endl;

    // Test dot product
    double dot = v2 * v3;
    cout << "v2 * v3 (dot product): " << dot << endl;

    // Test scalar multiplication
    Vector v6 = v2 * 2.0;
    cout << "v2 * 2.0: " << v6 << endl;

    Vector v7 = 3.0 * v2;
    cout << "3.0 * v2: " << v7 << endl;

    return 0;
}