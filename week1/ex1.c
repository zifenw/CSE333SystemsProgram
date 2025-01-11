/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/08/25
 */
#include <stdio.h>  // for printf
#include <stdint.h> // for int64_t and uint64_t
#include <stdlib.h> // for exit
#define ARRAY_SIZE 12

// Function Declaration
void CopyAndSort(int64_t *src, int64_t *dst, uint64_t size);
void insert(int64_t num, int64_t *array, uint64_t size);
void print(int64_t *array, uint64_t size);

int main(int argc, char **argv){
    int64_t original[ARRAY_SIZE] = {3, 2, -5, 7, 17, 42, 6, 333, 7, 8, -8, 6};
    int64_t sortCopy[ARRAY_SIZE];
    CopyAndSort(original, sortCopy, ARRAY_SIZE);
    print(sortCopy, ARRAY_SIZE);
    return EXIT_SUCCESS;
}

// Copies and sorts the array
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

// Prints the array
void print(int64_t *array, uint64_t size){
    for (int64_t i = 0; i < size; i++){
        printf("%ld%c", array[i], i == (size - 1) ? '\n' : ' ');
    }
}

