## Exercise 6
### Goals
学习如何使用标准C文件I/O库的各个部分，特别是在以后的项目中需要的部分。此外，还要重新强制理解存储在磁盘上的文件中的内容。

### Description
编写一个接受文件名作为单个命令行参数的C程序。程序应该读取文件，将文件内容以相反顺序逐字符（或者更准确地说逐字节）复制到`stdout`。

For example:
```
hello!
// Running the program should result in the following:
bash$ ./ex6 foo.txt

!ollehbash$
```
请注意换行符和其他字符在此输出中的位置。

- `fopen`  模式为“rb”，打开文件
- `fclose` 关闭文件
- `fseek` and `ftell` 计算文件的大小. (We'd suggest you fseek(f, 0, SEEK_END); and then size = ftell(f); to get the file size, in bytes.)
- `fseek` and `fread` 按相反顺序逐字节读取文件内容.
- `printf`, 使用%c格式代码，一次打印一个文件字符（字节）.

您的代码应该一次读取一个字节并立即打印它们。不要将整个文件读入内存并向后打印，也不要创建数据的反向副本并进行打印。本练习的目标是获得使用上述各种文件I/O功能的经验。

注意：其中一些库函数非常旧，并且具有类型较长的参数或结果。样式指南和cpplint之类的程序通常不鼓励使用long，而建议使用int64\t之类的类型。但是对于这些库函数，使用long类型的变量来匹配函数参数和结果类型是合适的，因为无论底层机器上long的确切大小如何，这都能保证精确匹配。在这些情况下，可以忽略任何关于使用long的cpplint警告。

提示：您可以使用诸如hextump-C foo.txt或hextump-C foo.txt之类的linux命令查看文件foo.txt中的字节（以十六进制和其他格式），以准确查看其中的内容，包括不可见字符。许多文本编辑器也有这样做的模式。
### fopen()
```c
FILE *fopen(const char *filename, const char *mode);
```
- "r"	以只读模式打开文件。如果文件不存在，返回 NULL。
- "w"	以写入模式打开文件。如果文件存在，清空文件内容；如果文件不存在，则创建新文件。
- "a"	以追加模式打开文件。如果文件不存在，则创建新文件。
- "r+"	以读写模式打开文件。如果文件不存在，返回 NULL。
- "w+"	以读写模式打开文件。如果文件存在，清空文件内容；如果文件不存在，则创建新文件。
- "a+"	以读写模式打开文件，文件指针指向文件末尾。如果文件不存在，则创建新文件。
- "rb"	以二进制模式只读打开文件（适用于处理非文本文件，如图像或音频）。
- "wb"	以二进制模式写入打开文件（如果文件存在，清空内容；不存在则创建）。
- "ab"	以二进制模式追加打开文件（文件指针指向末尾）。
### fseek()
```c
int fseek(FILE *stream, long offset, int whence);
```
1. stream:
- 指向文件的指针，通常由 fopen() 返回。
2. offset:
- 相对于 whence 的偏移量，单位是字节。
- 可以是正数（向前移动）、负数（向后移动）、或零（不移动）。
3. whence:
- 基准位置，决定偏移量的起始点。可选值：
  - SEEK_SET: 文件开头。
  - SEEK_CUR: 当前文件指针的位置。
  - SEEK_END: 文件末尾。

返回值  
- 成功: 返回 0。
- 失败: 返回非零值（通常为 -1），并设置全局变量 errno。
### ftell()
```c
long ftell(FILE *stream);
```
返回值  
- 成功: 返回一个 long 类型的值，表示当前文件指针的位置（从文件开头算起，单位为字节）。  
- 失败: 返回 -1L，并设置全局变量 errno，指示错误原因。
### fread()
```c
size_t fread(void *ptr, size_t size, size_t count, FILE *stream);
```
`ptr`: 指向接收数据的内存缓冲区的指针。  
`size`: 每个元素的大小（以字节为单位）。   
`count`: 读取的元素数量。   
`stream`: 文件指针，指向已打开的文件。  

返回值:
- 返回成功读取的元素数量。
- 如果返回值小于 count，可能是遇到了文件末尾（EOF）或发生了错误。

### test in Linux VM gcc
```
gcc -Wall -std=c17 -g -o ex6 ex6.c
./ex6 foo.txt
valgrind --leak-check=full ./ex6
python3 cpplint.py --clint ex6.c