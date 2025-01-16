/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/15/25
 */
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
        // EXIT_FAILURE if we've reached the largest positive integer
        if (checknext == INT64_MAX) {
            fprintf(stderr, "Hit the maximum value of int64_t.\n");
            exit(EXIT_FAILURE);
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