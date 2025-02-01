/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/27/31
 */
#include <iostream>
#include "Vector.h"

using std::cout;
using std::endl;
using vector333::Vector;

int main() {
    // test constructors and stream output
    Vector v1;
    cout << "Vector v1;\n";
    cout << "v1 should be (0,0,0): " << v1 << endl;

    Vector v2(1, 2, 3);
    cout << "Vector v2(1,2,3);\n";
    cout << "v2 should be (1,2,3): " << v2 << endl;

    Vector v3(v2);
    cout << "Copy constructor: Vector v3(v2);\n";
    cout << "v3 should be (1,2,3): " << v3 << endl;

    // test assignment (including chaining)
    // now v1(0,0,0), v2 (1,2,3), v3 (1,2,3)
    Vector v4, v5(2, -1, 4);
    cout << "Vector v4, v5(2,-1,4);\n";
    v3 = v4 = v5;
    cout << "v3 = v4 = v5;\n";
    cout << "Three copies of (2,-1,4):\n";
    cout << " v3: " << v3 << " v4: " << v4 << " v5: " << v5 << endl;

    v2 += v4;
    cout << "Update assignment: v2 += v4;\n";
    cout << "(1,2,3)+=(2,-1,4):\n";
    cout << " v2: " << v2 << " v4: " << v4 << endl;

    v1 -= v4;
    cout << "Update assignment: v1 -= v4;\n";
    cout << "(0,0,0)-=(2,-1,4):\n";
    cout << " v1: " << v1 << " v4: " << v4 << endl;

    // element-wise arithmetic
    cout << "Arithmetic:" << endl;
    cout << "  " << v1 << " + " << v2 << " = " << v1 + v2 << endl;
    cout << "  " << v1 << " - " << v2 << " = " << v1 - v2 << endl;

    // dot product
    cout << "Dot product: (a,b,c) * (x,y,z) = (ax+by+cz)" << endl;
    cout << "  " << v2 << " * " << v2 << " = " << v2 * v2 << endl;

    // scalar product
    cout << "Scalar product: (a,b,c) * k = (ak,bk,ck)" << endl;
    cout << "  " << v1 << " * 2 = " << v1 * 2 << endl;

    return 0;
}
