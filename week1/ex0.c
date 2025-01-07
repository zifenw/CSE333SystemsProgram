/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/06/25
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define constants
#define INITIAL_ESTIMATE 3.0
#define INITIAL_TERM 4.0
#define SIGN -1

//an example of how the user should invoke it
void PrintErrorMessage(void);

// Return the nth fractional term in the Nilakantha series.
double nthNilakantha(uint64_t n);

int main(int argc, char **argv){
    // check if user's input is exact one argument
    if (argc != 2) {
        PrintErrorMessage();
    }

    // store user's input
    uint64_t arg;
    if (sscanf(argv[1], "%llu", (unsigned long long *) &arg) != 1) {
        PrintErrorMessage();
    }

    // Calculate pi and exit
    double estimate = INITIAL_ESTIMATE;
    for (uint64_t i = 1; i<= arg; i++) {
        estimate += nthNilakantha(i);
    }
    printf("Our estimate of Pi is %.20f\n", estimate);

    return EXIT_SUCCESS;
}

double nthNilakantha(uint64_t n){
    double term = INITIAL_TERM; // Initialize term to 4.0
    term /= ((2.0 * n) * (2.0 * n + 1) * (2.0 * n + 2));
    if(n % 2 == 0){
        term *= SIGN;  // SIGN -1
    }
    return term;  
}

void PrintErrorMessage(void){
    fprintf(stderr,
            "Use n terms Nilakantha series to find pi" \
            "Input Format: ./ex0 n, where n>=0.\n");
    exit(EXIT_FAILURE);
}

