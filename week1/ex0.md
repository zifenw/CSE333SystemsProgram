## CSE 333 25wi Exercise 0
out: Monday, January 6, 2025
due: Wednesday, January 8, 2025 by 10:00 am. No late exercises will be accepted.

Goals: Write a C program from scratch including a function declaration, learn how to process command-line arguments, and explore some basic C libraries for handling I/O, strings, and numeric conversions.

Description: One way to estimate π is to use the following infinite series, which was discovered by Nilakantha in the 15th century:

      π = 3 + (4 / (2 x 3 x 4)) - (4 / (4 x 5 x 6)) + (4 / (6 x 7 x 8)) - ...
Breaking the series down:

the zero'th term in the series is 3
the first term in the series is +(4 / (2 x 3 x 4))
the second term in the series is -(4 / (4 x 5 x 6))
the nth term in the series is (-1)(n+1) x (4 / (2n x (2n+1) x (2n+2)))
Write a C program that estimates π by adding together terms 0 through n, inclusive, in the Nilakantha series, and prints out that estimate to 20 decimal places. "n" is provided to your program as a command-line argument. Your program, when compiled, should be called "ex0" and an example of how the user should invoke it, and the resulting output, which you should match, is:

    bash$ gcc -Wall -g -std=c17 -o ex0 ex0.c
    bash$ ls
    ex0     ex0.c
    bash$ ./ex0 100
    Our estimate of Pi is 3.14159241097198238535
    bash$
You should use C's double type for the floating-point arithmetic. Different floating-point types or adding up the terms in the series in an order different from left-to-right might yield different results. The point of this exercise, however, is to create and run a simple C program, not to explore the nuances of floating-point arithmetic, so please use the double datatype, which is adequate for our purposes and will produce the expected results, which should match the example.

Your code must:

compile without errors or warnings on CSE Linux machines (lab workstations, attu, or CSE home VM)
have no crashes, memory leaks, or memory errors
behave reasonably if given unreasonable input -- if the program argument is not an integer or is too large or too small to be handled properly, the program should do something sensible, and this behavior should be (briefly) documented in the code. Note: this is not intended to be an elaborate exercise in string handling. It should be enough to verify that there is a single argument that can be converted to a binary int and that it has a reasonable value.
be contained in a single file called "ex0.c" that compiles with the command gcc -Wall -g -std=c17 -o ex0 ex0.c -- do not submit a Makefile. Note that this means, although you are free to use any standard library functions, the libraries must not require you to add options to this command. In particular, using <math.h> requires the addition of the -lm option to the command, so you may not use this library.
be pretty: the formatting, modularization, variable and function names, and so on must make us smile rather than cry.
be robust: you should think about handling bogus input from the user, and you should handle hard-to-handle cases (if there are any) gracefully.
have a comment at the top of your .c file with your name and CSE or UW email address.
You should submit your exercise using the Gradescope dropbox linked on the course resources web page.

(You should have received an email message from Gradescope with your login information by the evening of the first class day. Your userid is your @uw.edu email address. If your gradescope account is not set up properly, please send email to cse333-staff[at]cs... with your uw netid so we can fix the problem. To submit file(s) using Gradescope, simply select the course, select the assignment, and drag the file(s) to the indicated place.)

A note on C Documentation
As part of this exercise you will need to explore some of the basic C libraries and conventions in order to access the command line argument (the string of digit characters), convert that digit string to a binary int value, and print the result. The course Resources page contains a link to www.cplusplus.com/reference, which is a good source for information about the C and C++ libraries (ignore the C++ information for now). In particular, you will find it useful to look at the stdio.h library for basic input and output, string.h for handling C strings (null-terminated arrays of characters), and stdlib.h for some useful functions, include parsing (converting) strings of digit characters to int values (there are multiple ways to do this later task, including sscanf, atoi, and others -- think about the tradeoffs and differences between them).
### c, c++, gcc环境设置
```
环境变量
A:\MinGW\bin

```
### Linux创建测试文件流程
```
ssh zifenw@attu.cs.washington.edu
mkdir cse333
ls
cd cse333
vim ex0.c
```
### c/c++ Library reference
```

1. #include <stdio.h>：标准输入/输出库
<stdio.h> 提供了处理输入和输出操作的函数，比如从控制台读写数据或与文件交互。

常用函数
printf()	向控制台输出格式化的内容。
scanf()	从控制台读取格式化的输入。
fprintf()	向文件输出格式化的内容。
fscanf()	从文件读取格式化的内容。
fgets()	从文件或输入中读取字符串。
fputs()	将字符串写入文件。
fopen()	打开一个文件。
fclose()	关闭文件。
fseek()	设置文件的当前位置指针。
ftell()	返回当前文件指针的位置。

2. #include <stdlib.h>：标准库
<stdlib.h> 提供了通用的工具函数，包括内存管理、随机数生成和程序控制等功能。

常用函数
函数	功能描述
malloc()	动态分配内存。
free()	释放动态分配的内存。
calloc()	分配内存并初始化为零。
realloc()	调整已分配内存的大小。
rand()	生成随机数。
srand()	设置随机数生成器的种子。
exit()	退出程序。                  <-使用此功能
atoi()	将字符串转换为整数。
atof()	将字符串转换为浮点数。
abs()	返回整数的绝对值。

3. #include <stdint.h>: 整数类型库
<stdint.h> 提供了一组具有特定位宽的整数类型，使它们在不同的平台和编译器之间具有可移植性。
uint64_t 64位无符号整数类型         <-使用此功能
范围：0 到 18,446,744,073,709,551,615
```
### Define constants
```
#define INITIAL_ESTIMATE 3.0
#define INITIAL_TERM 4.0
#define SIGN -1
```
### nthNilakantha(uint64_t n) method
```
double nthNilakantha(uint64_t n){
    double term = INITIAL_TERM; // Initialize term to 4.0
    term /= ((2.0 * n) * (2.0 * n + 1) * (2.0 * n + 2));
    if(n % 2 == 0){
        term *= SIGN;  // SIGN -1
    }
    return term;  
}
``` 
### PrintErrorMessage(void) method
```
与java不同，c不可以省略void写成void PrintErrorMessage(){}

int fprintf(FILE *stream, const char *format, ...);
stream：这是输出流，可以是标准输出流（stdout）、标准错误流（stderr）、或者是文件流（FILE* 类型）
format：这是一个格式化字符串，和 printf 类似，用来指定输出的格式。
...：这些是要输出的参数，具体的内容取决于格式化字符串中的占位符（如 %d、%s 等）。

void PrintErrorMessage(void){
    fprintf(stderr,
            "Use n terms Nilakantha series to find pi" \
            "Input Format: ./ex0 n, where n>=0.\n");
    exit(EXIT_FAILURE);
}
```
#### main()
```
// check if user's input is exact one argument
    if (argc != 2) {
        PrintErrorMessage();
    }

使用 sscanf 解析参数

sscanf(argv[1], "%llu", (unsigned long long *) &res)
-argv[1]：指向命令行传递的第一个参数（不包括程序名 argv[0]）。
-"%llu"：格式说明符，用于解析无符号长长整型（unsigned long long）值。
-(unsigned long long *) &res：将变量 res 的地址强制转换为 unsigned long long *，以匹配 sscanf 的要求。
-返回值：sscanf 返回成功匹配的项目数。如果能够成功解析 argv[1] 为一个无符号整数，返回值为 1。

 // store user's input
    uint64_t arg;
    if (sscanf(argv[1], "%llu", (unsigned long long *) &arg) !=1) {
        PrintErrorMessage();
    }

 // Calculate pi and exit
    double estimate = INITIAL_ESTIMATE;
    for (uint64_t i = 1; i<= arg; i++) {
        estimate += nthNilakantha(i);
    }
    printf("Our estimate of Pi is %.20f\n", estimate);

    return EXIT_SUCCESS;
```