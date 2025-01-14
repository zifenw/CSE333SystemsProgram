## structs, typedef, and dynamic memory allocation

### typedef
Allows you to define new data type names/synonyms

### struct
Use “.” to refer to a field in a struct  
Use “->” to refer to a field from a struct pointer  
p -> x = (p*).x


- uses `typedef` to define a new `structured` type called Point3d, which contains `uint16_t` fields for x, y, and z coordinates.


```c
#include <stdint.h>

typedef struct Point3d_st{
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Point3d, *Point3dPtr;
// Point3dPtr is equivalent to Point3d*
```

- defines a function called AllocatePoint3d that accepts three uint16_t values as arguments, mallocs space for a Point3d, assigns those three arguments to the x, y, and z fields, and returns (a pointer to) the malloc'ed Point3d.
```c
Point3dPtr AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z);

Point3dPtr AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z){
    // malloc for a new Point3d
    Point3dPtr retval = (Point3dPtr) malloc (sizeof(Point3d));
    // assign given value to the new Point3d if malloc success
    if (retval != NULL){
        retval->x = x;
        retval->y = y;
        retval->z = z;
    }
    return retval;
}
//retval (means return value)
```

- has a main() that "tests" AllocatePoint3d. For the purposes of this exercise, it is enough for you to write code that simply allocates a struct by calling AllocatePoint3d and verifies that the struct members contain the correct values. Your "test" should print an appropriate message if something is wrong; if all is well your program should terminate silently. Make sure your main function frees any memory that AllocatePoint3d allocates. Hint: Don't make assumptions about what AllocatePoint3d does, even though you implement it. Imagine that your main is testing an implementation of AllocatePoint3d that is implemented by another user.


According to the C standard, the `main` function should return an `int`.

`int fprintf(FILE *stream, const char *format, ...);`   
`FILE *stream`: 指定输出流，可以是标准输出流（stdout）、标准错误流（stderr）或者通过 fopen 打开的文件流.  
`const char *format`: 格式字符串，用于指定输出内容的格式。可以包括：
普通文本, 格式说明符（如 %d, %f, %s 等），用于插入变量的值。  
`...`: 对应格式说明符的参数。


```c
//al
int main(int argc, char** argv) {
    Point3dPtr ptr = AllocatePoint3d(1,13,2025);
    if (ptr == NULL) {
        fprintf(stderr, "Function failed.\n");
        return EXIT_FAILURE;
    }

    // Check that the triple fields have proper values
    if (ptr->x != 1 || ptr->y != 13 || ptr->z != 2025) {
        fprintf(stderr, "Wrong value for point fields\n");
        return EXIT_FAILURE;
    }

    printf("AllocatePoint3d() works fine.\n");

    free(ptr);
    return EXIT_SUCCESS;
}
```

### compiled and test
```
gcc -o ex3 ex3.c
./ex3
```
### test in Linux VM gcc
```
ssh zifenw@attu.cs.washington.edu
gcc -Wall -std=c17 -g -o ex3 ex3.c
valgrind --leak-check=full ./ex3
python3 cpplint.py --clint ex3.c