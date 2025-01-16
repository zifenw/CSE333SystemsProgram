/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/15/25
 */
#ifndef _NTHPRIME_H_
#define _NTHPRIME_H_

#include <stdint.h>  // for int16_t and int64_t

// The function should return the nth prime number, 
// where n is the function's parameter. 
// The function call 
// NthPrime(1) should return 2, 
// NthPrime(2) should return 3, 
// NthPrime(3) should return 5, and so on.
// n must > 0

int64_t NthPrime(int16_t n);

#endif  // _NTHPRIME_H_