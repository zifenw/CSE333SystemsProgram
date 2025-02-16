/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/14/25
 */
#include <iostream>
#include "Cartesian.h"
#include "Polar.h"

#define PI 3.1415927

using std::cout;
using std::endl;

int main(int argc, char **argv) {
  // Cartesian coordinates test
  Cartesian c1(0, 6.0);  // c1(0, 6)
  Cartesian c2(-8.0, 0);  // c2(-8, 0)
  cout << "C1: " << c1.ToString() << endl;
  cout << "C2: " << c2.ToString() << endl;
  cout << "The distance between c1 and c2 is " << c1.Distance(c2) << endl;
  cout << endl;

  // Polar coordinates test (radians)
  Polar p1(3.0, 0);  // p1(3, 0), angle in radians = 0
  Polar p2(4.0, PI / 2);  // p2(4, π/2 radians), which corresponds to 90 degrees
  cout << "P1: " << p1.ToString() << endl;
  cout << "P2: " << p2.ToString() << " means (4, π/2)" << endl;
  cout << "The distance between p1 and p2 is " << p1.Distance(p2) << endl;

  return EXIT_SUCCESS;  // success
}
