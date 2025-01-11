- convince the compiler to let you access bytes in memory, given the address of the first byte to be printed as a void* pointer value. 说服编译器允许您访问内存中的字节，给定要打印为void*指针值的第一个字节的地址。
- use a loop to iterate once for each byte in the variable, using a pointer to extract that byte and print it. 使用循环对变量中的每个字节迭代一次，使用指针提取并打印该字节。
- figure out how to use format specifiers to printf in order to print out an uint8_t byte in lower-case hexadecimal, using exactly two digits.了解如何使用格式说明符来printf，以便使用两位数字以小写十六进制打印uint8\u t字节。   
As a hint, take inspiration from this code:
```
#include <inttypes.h>
...
uint8_t a_byte = 0xD1;
printf("The byte is: %02" PRIx8 " -- enjoy!\n", a_byte);
...
```
- figure out how to print a pointer value. 了解如何打印指针值。

## Important Function
```c
void PrintBytes(void* mem_addr, int num_bytes){
// void* mem_addr generic pointer
// void doesn’t have a size!

// num_bytes sizeof(type)
printf("The %d bytes starting at %p are: ", num_bytes, mem_addr);
// %d: 用于格式化输出整数值。num_bytes 是要打印的字节数。
// %p: 用于格式化输出指针地址。mem_addr 是一个指向内存地址的指针。
for(int i = 0; i<num_bytes; i++){
    printf("%02x ", ptr[i]);
}

//x: 按十六进制格式输出整数。
}
```
## printf()
`printf` 格式说明符
1. 整数  
`%d`	以十进制打印有符号整数	`printf("%d", -10);` 输出：-10  
`%u`	以十进制打印无符号整数	`printf("%u", 10);` 输出：10  
`%x`	以十六进制打印无符号整数 (小写)	`printf("%x", 255);` 输出：ff  
`%X`	以十六进制打印无符号整数 (大写)	`printf("%X", 255);` 输出：FF  
`%o`	以八进制打印无符号整数	`printf("%o", 10);` 输出：12  

2. 浮点数  
`%f`	打印小数 (默认 6 位精度)	`printf("%f", 3.14);` 输出：3.140000  
`%.2f`	打印小数（保留 2 位小数）	`printf("%.2f", 3.14);` 输出：3.14  
`%e`	以科学计数法打印小数	`printf("%e", 314.0);` 输出：3.140000e+02  

3. 字符与字符串  
`%c`	打印单个字符	`printf("%c", 'A');` 输出：A  
`%s`	打印字符串	`printf("%s", "Hello");` 输出：Hello

4. 指针与特殊符号  
`%p`	打印指针地址	`printf("%p", &val);` 输出：0x7ffee1aabc10
`%%`	打印百分号 %	`printf("%%");` 输出：%

5. `0`: 填充字符
```c
printf("%02d\n", 5);  // 输出：05 （不足两位，前面补 0）
printf("%2d\n", 5);   // 输出： 5 （不足两位，前面补空格）
```

## 可移植性
`#include <inttypes.h>` 包含`#include <stdint.h>`   
`PRIxPTR`: 用于格式化与指针大小一致的整数类型（uintptr_t）。不同平台上，指针的大小可能是 32 位或 64 位。    
`PRIx8`: 用于格式化 8 位无符号整数 `uint8_t`，保证了无论平台如何，都能正确输出 uint8_t 类型数据为十六进制格式。  
`uintptr_t` 是一种无符号整数类型，可以存储任何指针的值  
`int32_t` 确保无论在哪个平台上，都会分配 4 字节存储空间，并且始终代表一个 32 位的有符号整数。

### compiled and test
```
gcc -o ex2 ex2.c
./ex2
The 1 bytes starting at 0x61fe1f are: 30
The 4 bytes starting at 0x61fe18 are: 01 00 00 00
The 4 bytes starting at 0x61fe14 are: 00 00 80 3f
The 8 bytes starting at 0x61fe08 are: 00 00 00 00 00 00 f0 3f
The 24 bytes starting at 0x61fdf0 are: 30 00 00 00 01 00 00 00 00 00 80 3f 00 00 00 00 00 00 00 00 00 00 f0 3f
```
### test in Linux VM gcc
```
ssh zifenw@attu.cs.washington.edu
