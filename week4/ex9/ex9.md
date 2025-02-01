## Goals
Create and use a complete C++ class for a new 3-D Vector abstraction, including `constructors`, `destructors`, operators including `assignment` and `stream output`, and a `simple user-defined namespace`.
## Description
- In file `Vector.h` declare a class `Vector` with the following properties:

  - The representation of a Vector should be three doubles giving the magnitudes in the x, y, and z directions.向量的表示应为三个双精度浮点数，分别表示x、y和z方向上的大小。
  - There should be `a default (0-argument) constructor` that initializes a Vector to (0,0,0), `a constructor with 3 doubles as parameters` giving initial values for the x, y, and z magnitudes, and `a copy constructor`. 应该有一个默认的（0参数）构造函数，将Vector初始化为（0,0,0），一个具有3个参数的构造函数，为x、y和z幅度提供初始值，以及一个复制构造函数。
  - There should be `a destructor` that does whatever work is needed when a Vector object is deleted. If no work is needed, the body of the destructor can be empty. 应该有一个析构函数，在删除Vector对象时执行所需的任何工作。如果不需要任何工作，析构函数的主体可以为空。
  - There should be `three accessor functions` get_x(), get_y(), and get_z() that return the values of the x, y, and z coordinates of the Vector, respectively. 应该有三个访问函数get_x（）、get_y（）和get_z（），分别返回Vector的x、y和z坐标值。
  - The class should define `assignment for Vectors (u=v)`. 该类应定义Vector（u=v）的赋值。
  - The class should define `updating assignment for Vectors (u+=v and u-=v)` that perform element-by-element addition or subtraction of the Vector components. 该类应定义Vector（u+=v和u-=v）的更新赋值，这些Vector执行Vector分量的逐元素加法或减法。
  - Operators `+ and - should be overloaded` so that u+v and u-v return new Vectors that are the sum and difference of Vectors u and v, respectively. 运算符+和-应重载，以便u+v和u-v分别返回向量u和v的和和和差。
  - `Operator *` should compute the inner product (dot product) of two Vectors. If v1=(a,b,c) and v2=(d,e,f), then v1\*v2 should return the scalar value a\*d+b\*e+c\*f as a double. 运算符\*应计算两个向量的内积（点积）。如果v1=（a，b，c）和v2=（d，e，f），则v1\*v2应返回标量值a\*d+b\*e+c\*f作为double。
  - Operator `* should be overloaded` so that if v is the Vector (a,b,c) and k is a double, then v\*k and k\*v should return new Vectors containing the components of v multiplied by k (i.e., (a\*k,b\*k,c\*k)). 运算符\*应该重载，这样如果v是Vector（a，b，c），k是double，那么v\*k和k\*v应该返回包含v乘以k的分量的新Vector（即（a\*k，b\*k，c\*k））。
  - The class should define `stream output` so that $s<<v$ will write Vector v to stream s as (a,b,c), i.e., a left parentheses followed by the x, y, and z components of v separated by commas only, and a right parentheses. There should be no added spaces in the output. 该类应定义流输出，以便 $s<<v$ 将Vector v写入流s为（a，b，c），即左括号后跟v的x、y和z分量，仅用逗号分隔，右括号。输出中不应添加空格。
  - The Vector class and associated functions should be placed in a `namespace vector333`. Vector类和相关函数应放置在命名空间vector333中。

**Note that several of these functions are required to return new Vectors. This means actual Vector values, not pointers or references to Vectors that have been allocated elsewhere.**

- In file `Vector.cc` implement this class. 在文件Vector.cc中实现这个类。
- In file `ex9.cc` write a main program that demonstrates that your Vector class works properly. The output format is up to you, but it should be labeled neatly and should be concise so it can be read with pleasure, not pain. It should show the operands and results of Vector operations, such as (1,-2,3)+(1,1,1)=(2,-1,4) so the reader can verify that the Vector operations work properly. Feel free to write using namespace vector333; in your test program so you don't need to type vector333:: repeatedly. 在文件ex9.cc中编写一个主程序，演示Vector类是否正常工作。输出格式由您决定，但它应该整齐地标记，并且应该简洁明了，这样就可以愉快地阅读，而不是痛苦地阅读。它应该显示Vector运算的操作数和结果，例如 （1，-2,3）+（1,1,1）=（2，-1,4），以便读者可以验证Vector运算是否正常工作。请随意使用名称空间vector333编写；在测试程序中，这样您就不需要重复键入vector333:：。
- Create a suitable `Makefile`. The command make should build an executable program ex9, recompiling individual files only when needed. The command make clean should remove the ex9 executable file, all .o files, and any editor or other backup files whose names end in ~ . make命令应构建一个可执行程序ex9，仅在需要时重新编译单个文件。make clean命令应删除ex9可执行文件、所有.o文件以及名称以~结尾的任何编辑器或其他备份文件。

```c++
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
        // constructors with initialization list

        // default constructor
        Vector() : x_(0.0), y_(0.0), z_(0.0) { }
        // construct the vector (x,y,z)
        Vector(const double x, const double y, const double z)
            : x_(x), y_(y), z_(z) { }
        // copy constructors
        Vector(const Vector& copyme)
            : x_(copyme.x_), y_(copyme.y_), z_(copyme.z_) { }
        // accessor functions
        double get_x() const { return x_; }
        double get_y() const { return y_; }
        double get_z() const { return z_; }

        // destructor
        ~Vector() { }

    private:
        double x_, y_, z_; // // data member
    }; // class Vector

}  // // namespace vector333

#endif // VECTOR_H_
```

```

```