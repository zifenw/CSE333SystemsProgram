/*
 * Copyright ©2025 Hal Perkins.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2025 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

#include <stdio.h>       // for printf
#include <stdlib.h>      // for EXIT_SUCCESS
#include <inttypes.h>    // for uint8_t, PRIx8, etc.

// Print num_bytes in decimal and num_bytes bytes in hex starting at mem_addr
void PrintBytes(void *mem_addr, int num_bytes) {
  uint8_t* ptr = (unsigned char*)mem_addr;

  printf("The %d bytes starting at 0x%"PRIxPTR " are: ",
              num_bytes, (uintptr_t)ptr);
  for (int i = 0; i < num_bytes; i++) {
    printf("%s%02" PRIx8, (i > 0 ? " " : ""), ptr[i]);
  }
  printf("\n");
}

int main(int argc, char **argv) {
  char     char_val = '0';
  int32_t  int_val = 1;
  float    float_val = 1.0;
  double   double_val  = 1.0;

  typedef struct {
    char    char_val;
    int32_t int_val;
    float   float_val;
    double  double_val;
  } Ex2Struct;

  Ex2Struct struct_val = { '0', 1, 1.0, 1.0 };

  PrintBytes(&char_val, sizeof(char));
  PrintBytes(&int_val, sizeof(int32_t));
  PrintBytes(&float_val, sizeof(float));
  PrintBytes(&double_val, sizeof(double));
  PrintBytes(&struct_val, sizeof(struct_val));

  return EXIT_SUCCESS;
}