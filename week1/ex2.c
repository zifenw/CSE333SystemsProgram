/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/10/25
 */
#include <stdio.h> // for printf
#include <stdlib.h> // for EXIT_SUCCESS
#include <inttypes.h> // for uint8_t, PRIx8, PRIxPTR, int32_t

void PrintBytes(void* mem_addr, int num_bytes);

int main(int argc, char **argv) {
  char     char_val = '0';
  int32_t  int_val = 1;
  float    float_val = 1.0;
  double   double_val  = 1.0;

  typedef struct {
    char     char_val;
    int32_t  int_val;
    float    float_val;
    double   double_val;
  } Ex2Struct;

  Ex2Struct struct_val = { '0', 1, 1.0, 1.0 };

  PrintBytes(&char_val, sizeof(char));
  PrintBytes(&int_val, sizeof(int32_t));
  PrintBytes(&float_val, sizeof(float));
  PrintBytes(&double_val, sizeof(double));
  PrintBytes(&struct_val, sizeof(struct_val));

  return EXIT_SUCCESS;
}

void PrintBytes(void* mem_addr, int num_bytes){
    //casting the void* (generic pointer)
    uint8_t* ptr = (uint8_t*) mem_addr;

    printf("The %d bytes starting at 0x%" PRIxPTR " are: ", num_bytes, (uintptr_t)mem_addr);
    
    // %s   (i > 0)? " " : "" add space befor each bytes except first one
    // %02" PRIx8   two-digit hexadecimal format, zero-padded if necessary
    for(int i = 0; i < num_bytes; i++){
        printf("%s%02" PRIx8, (i > 0)? " " : "", ptr[i]);
    }
    printf("\n");
}