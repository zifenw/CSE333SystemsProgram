/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/15/25
 */
#include <stdio.h>  // for printf
#include <stdlib.h>  // for EXIT_SUCCESS
#include <inttypes.h>   // for PRId64

#include "NthPrime.h"

int main(int argc, char **argv) {
  printf("The 15th prime is: %" PRId64 ".\n", NthPrime(15));
  printf("The 333th prime is: %" PRId64 ".\n", NthPrime(333));
  return EXIT_SUCCESS;
}