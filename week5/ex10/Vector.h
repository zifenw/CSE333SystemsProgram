/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/03/31
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
using std::ostream;

namespace vector333 {

// A Vector represents a vector in 3-space.
class Vector {
 public:
  // constructors:
  // Default constructor
  Vector();
  // Parameterized constructor
  Vector(const double x, const double y, const double z);
  // Copy constructor
  Vector(const Vector &v);

  // Destructor - nothing to do
  ~Vector();

  // Assignment
  Vector &operator=(const Vector &rhs);

  // Updating assignment
  Vector &operator+=(const Vector &rhs);
  Vector &operator-=(const Vector &rhs);

  // Friend functions for operators
  friend Vector operator+(const Vector &v1, const Vector &v2);
  friend Vector operator-(const Vector &v1, const Vector &v2);
  friend double operator*(const Vector &v1, const Vector &v2);
  friend Vector operator*(const Vector &v, const double k);
  friend Vector operator*(const double k, const Vector &v);

  // Stream output
  friend std::ostream &operator<<(std::ostream &os, const Vector &v);

 private:
  // The representation of a Vector is three doubles giving
  // the x, y, and z magnitudes.
  double *data_;
};

}  // namespace vector333

#endif  //  VECTOR_H_