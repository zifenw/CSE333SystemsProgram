/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/27/31
 */
#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

namespace vector333 {

class Vector {
 public:
  // default constructor
  Vector();

  // construct the vector (x,y,z)
  Vector(const double x, const double y, const double z);

  // copy constructors
  Vector(const Vector &v);

  // accessor functions
  double get_x() const;
  double get_y() const;
  double get_z() const;

  // assignment operators for Vector
  Vector &operator=(const Vector &v);

  // updating assignment for Vector
  Vector &operator+=(const Vector &v);
  Vector &operator-=(const Vector &v);

  // produce a new Vector by two Vector addition
  Vector operator+(const Vector &v);

  // produce a new Vector by two Vector subtraction
  Vector operator-(const Vector &v);

  // return a double value by two Vector dot-product
  double operator*(const Vector &v);

  // produce a new Vector by scalar multiplication
  Vector operator*(const double k);

  // Stream output of the Vector
  friend std::ostream &operator<<(std::ostream &os, const Vector &v);

  // destructor
  ~Vector();

 private:
  double x_, y_, z_;  // data member
};  // class Vector

}  // namespace vector333

#endif  // VECTOR_H_