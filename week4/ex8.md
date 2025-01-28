## CSE 333 24wi Exercise 8

### Goals
从头开始编写一个小型的过程式（没有类或对象）C++程序，学习C++流I/O的基础知识，并学习使用cpplint风格的C++代码检查器。
### Description 
Write a C++ program that:  

- 提示用户输入一个正整数（严格大于0）
- 打印出作为该整数因子的所有整数，并在末尾添加换行符；您可以随意使用您能想到的最简单的暴力因子分解算法（提示：n%k==0），并且应该按升序只打印每个因子一次。例如，如果用户输入数字100，与终端的交互应该如下：
```
bash$ ./ex8
Which positive integer would you like me to factorize? 100
1 2 4 5 10 20 25 50 100
bash$ 
```
- Uses `std::cin` to read from the terminal and `std::cout` to write to the terminal


### Code
```c++
#include <iostream>   // For standard input and output
#include <cstdlib>    // For EXIT_SUCCESS and EXIT_FAILURE
#include <cinttypes>  // For fixed-width integer types like uint64_t

using namespace std;

int main(int argc, char** argv) {
    uint64_t input;
    cout << "Which positive integer would you like me to factorize? ";
    cin >> input; // Read user input

    // Check if the input is valid (not a non-integer value)
    if (!cin) {
        cerr << "Input is not a positive integer" << endl;
        return EXIT_FAILURE; // Exit with failure status
    }

    // Ensure input is not zero (factorization of 0 is undefined)
    if (input == 0) {
        cerr << "input cannot be 0" << endl;
        return EXIT_FAILURE; // Exit with failure status
    }

    // Loop through all numbers from 1 to the input value
    for (uint64_t i = 1; i <= input; i++) {
        if (input % i == 0) {
            // Print the factor, adding a space
            // before the number (except for the first factor)
            cout << (i == 1 ? "" : " ") << i;
        }
    }
    cout << endl; // Print a newline for better formatting
    return EXIT_SUCCESS; // Exit with success status
}
```
### Test
```
g++ -Wall -g -std=c++17 -o ex ex8.cc
cpplint ex8.cc

ssh zifenw@attu.cs.washington.edu
valgrind --leak-check=full ./ex8
python3 cpplint.py --clint ex8.cc
```