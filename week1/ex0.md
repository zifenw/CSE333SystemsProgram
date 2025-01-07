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