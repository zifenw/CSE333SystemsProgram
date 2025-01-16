### Goals
Write a multi-file C program that properly uses header, implementation, and client files to partition a program.


### Description
`NthPrime.h`: 一个头文件，包含一个名为NthPrime（）的函数的单个函数原型声明，以及原型上方的注释，说明如何使用该函数。函数应接受单个`int16_t`参数，并应返回`int16_t`。函数应返回第n个素数，其中n是函数的参数。函数调用NthPrime（1）应该返回2，NthPrime（2）应该返回3，NthPrime（3）应该返回5，依此类推。

`NthPrime.c`: 包含NthPrime实现的文件。请随意使用最简单的素性测试算法 (hint: "x is not prime if it is divisible by ...")。您也可以在这里定义一些辅助函数。

`ex4.c`: 一个包含main（）函数的文件，该函数通过为至少两个不同的、非平凡的参数打印NthPrime的输入和输出来测试NthPrime（但不需要打印大量的测试用例，请保持合理）。

头文件应包含适当的头保护，以正确处理多个包含`（#ifndef…）`  
您的实现文件应该使用static为任何私有“helper”函数提供适当的内部（`private`）链接

### ifndef 头文件保护的工作原理
1. `#ifndef` 和 `#define` 的作用：

- `#ifndef _NTHPRIME_H_` 检查宏 `_NTHPRIME_H_` 是否已经定义。
- 如果没有定义，执行接下来的内容，并通过 `#define _NTHPRIME_H_` 定义该宏。
- 如果已经定义，直接跳过整个文件内容，避免重复包含。
2. 避免重复定义错误：
- 如果多个源文件都包含了这个头文件，头文件保护机制可以确保文件内容只会被处理一次，从而避免函数或变量的重复定义错误。
3. #endif 的作用：
- 它对应于 #ifndef，表示条件编译的结束。
```c
#ifndef _NTHPRIME_H_
#define _NTHPRIME_H_

#include <stdint.h>  // for int16_t

// The function should return the nth prime number, 
// where n is the function's parameter. 
// The function call 
// NthPrime(1) should return 2, 
// NthPrime(2) should return 3, 
// NthPrime(3) should return 5, and so on.
// n must > 0

int16_t NthPrime(int16_t n);

#endif  // _NTHPRIME_H_
```

### ex4.c main test file
```c
#include <stdint.h>  // for printf
#include <stdlib.h>  // for EXIT_SUCCESS
#include <inttypes.h>   // for PRIu64

#include "NthPrime.h"

int main(int argc, char **argv) {
  printf("The 15th prime is: %" PRId16 ".\n", NthPrime(15));
  printf("The 333th prime is: %" PRId16 ".\n", NthPrime(333));
  return EXIT_SUCCESS;
}
/*PRId32: 用于打印 int32_t。
  PRIu64: 用于打印 uint64_t。
  SCNd16: 用于扫描（读取）int16_t。
  SCNu32: 用于扫描（读取）uint32_t。*/
```
### NthPrime
```c
#include <stdio.h>    // for printf
#include <stdlib.h>   // for exit, EXIT_SUCCESS
#include <stdbool.h>  // for bool

#include "NthPrime.h"

// Return true if n is prime, false otherwise.
bool IsPrime(int64_t n);

int64_t NthPrime(int16_t n){
    // print error if n <= 0
    if (n <= 0) {
        fprintf(stderr, 
        "error: input for NthPrime(n) must be a postive number\n");
        exit(EXIT_FAILURE);
    }

    int64_t checknext = 2;
    int64_t prime_count = 0;

    // get into the while loop until find nth prime
    while (true) {
        if (IsPrime(checknext)) {
            prime_count++;
            if (prime_count == n) {
                return checknext;
            }
        }
        checknext = (checknext == 2) ? 3 : checknext + 2;  // Skip even numbers after 2
    }
}

bool IsPrime(int64_t n) {
    // check whether num has a factor
    // If num can be divided by a number greater than sqrt (num),
    // there must also be a factor less than or equal to sqrt (num).
    for (int64_t i = 2; i *i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
```
### compiled and test
```
gcc -Wall -g -std=c17 -c -o NthPrime.o NthPrime.c
gcc -Wall -g -std=c17 -c -o ex4.o ex4.c
gcc -Wall -g -std=c17 -o ex4 ex4.o NthPrime.o
./ex4
```
### test in Linux VM gcc
```
ssh zifenw@attu.cs.washington.edu
gcc -Wall -g -std=c17 -c -o NthPrime.o NthPrime.c
gcc -Wall -g -std=c17 -c -o ex4.o ex4.c
gcc -Wall -g -std=c17 -o ex4 ex4.o NthPrime.o
valgrind --leak-check=full ./ex3
python3 cpplint.py --clint ex3.c