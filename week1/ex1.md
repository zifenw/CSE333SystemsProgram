## CSE 333 25wi Exercise 1
out: Wednesday, January 8, 2025
due: Friday, January 10, 2025 by 10:00 am. No late exercises accepted.

Goals: Implement and use a function with array parameters; process and print fixed-width integer data

Description: Write (in C) a function CopyAndSort that accepts two arrays of int64_t's and an array length as arguments. You should assume the lengths of the two arrays are the same. The function should iterate through the elements of the first array and use insertion sort to store them into the second array in non-decreasing (i.e., ascending, but there might be duplicates) sorted order. Your code should insert the array elements in the proper place one at a time as they are copied. Do not copy the entire array first and then sort it.

You should assume that the array lengths and subscripts can be stored in variables and passed as parameters of type int. When printing array values you should use the correct format for int64_t values rather than printf codes for built-in C types like int, short, or long (Hint: explore the stdint.h and inttypes.h libraries). You should decompose the problem into multiple functions if appropriate.

(Note about int: in systems code we very often will prefer to use types like int32_t with precisely known sizes, but we also want to follow the Google style guide, which says that if all you need is an int, you should use int. See the Google C++ style guide for more details. The information in there that is not C++-specific also applies to our C code. For this exercise we'll use arrays whose elements have explicitly-sized integer elements, but the array sizes and indices are ordinary integer values.)

Write a main() function that exercises your CopyAndSort function. When your program runs, it should sort the following array and print out the results:

     {3, 2, -5, 7, 17, 42, 6, 333, 7, 8, -8, 6}
When your program compiles and runs, we should see:
    bash$ gcc -Wall -g -std=c17 -o ex1 ex1.c
    bash$ ./ex1
    -8 -5 2 3 6 6 7 7 8 17 42 333
    bash$
Your code must:

compile without errors or warnings on CSE Linux machines (lab workstations, attu, or CSE home VM)
have no crashes, memory leaks, or memory errors on CSE linux machines
be contained in a single file called ex1.c.
be pretty: the formatting, modularization, variable and function names, and so on must make us smile rather than cry.
be robust: you should think about handling bogus input from the user (if any), and you should handle hard-to-handle cases (if there are any) gracefully.
have a comment at the top of your .c file with your name, and CSE or UW email address.
You should submit your exercise using the Gradescope dropbox linked on the course resources web page.

### main method for test
```
int main(int argc, char **argv){
    int64_t original[ARRAY_SIZE] =  {3, 2, -5, 7, 17, 42, 6, 333, 7, 8, -8, 6};
    int64_t sortCopy[ARRAY_SIZE];
    CopyAndSort(original, sortCopy, ARRAY_SIZE);
}

here we use the int64_t which need #include <stdint.h>

-argc contains the number of strings on the command line (the
executable name counts as one, plus one for each argument).

-argv is an array containing pointers to the arguments as strings
```
### All possible methods
```
void CopyAndSort(int64_t src[], int64_t dst[], uint64_t size){

}

void insert(int64_t num, int64_t *array, uint64_t size){

}

void print(int64_t *array, uint64_t size){

}
```
### Function Declaration
```
void CopyAndSort(int64_t src[], int64_t dst[], uint64_t size);
void insert(int64_t num, int64_t *array, uint64_t size);
void print(int64_t *array, uint64_t size);

Function Ordering Solution:
-Reverse Ordering
-function Declaration
```
### print()
```
void print(int64_t *array, uint64_t size){
    for(int64_t i = 0; i<size; i++){
        printf("%lld%c", array[i], i == (size - 1) ? '\n' : ' ');
    }
}

%lld 和%c 是格式化占位符,代表int64_t和char
%llu是 uint64_t

%lld:   array[i]
%c:     i == (size - 1) ? '\n' : ' ' 表示出最后一个换行外打印空格
```
### CopyAndSort() and insert()
```
void CopyAndSort(int64_t *src, int64_t *dst, uint64_t size){
    if(size == 0){
        return;
    }

    for (uint64_t i = 0; i < size; i++){
        insert(src[i], dst, i);
    }
}
// Inserts a number into the sorted part of the array
void insert(int64_t num, int64_t *array, uint64_t size){
    //Check the array from the last element.
    //If num is smaller, move the pld value to the next index
    uint64_t i = size;
    while (i > 0 && array[i - 1] > num){
        array[i] = array[i - 1];
        i--;
    }
    //Find the fit num place and insert 
    array[i] = num;
}
```
### compiled and test
```
gcc -o ex1 ex1.c
./ex1
-8 -5 2 3 6 6 7 7 8 17 42 333
```
### test in Linux VM gcc
```
ssh zifenw@attu.cs.washington.edu

ex1.c:52:20: warning: format ‘%lld’ expects argument of type ‘long long int’, but argument 2 has type ‘int64_t’ {aka ‘long int’} [-Wformat=]
   52 |         printf("%lld%c", array[i], i == (size - 1) ? '\n' : ' ');
      |                 ~~~^     ~~~~~~~~
      |                    |          |
      |                    |          int64_t {aka long int}
      |                    long long int
      |                 %ld

change "%lld" to "%ld"
```