/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/13/25
 */
#include <stdint.h>  // for fprintf
#include <stdlib.h>  // EXIT_SUCCESS, malloc, free
#include <stdio.h>   // uint16_t

// typedef structured Point3d for x, y, and z coordinates
typedef struct Point3d_st {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} Point3d, *Point3dPtr;

// Function Declaration
Point3dPtr AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z);

int main(int argc, char **argv) {
    Point3dPtr ptr = AllocatePoint3d(1, 13, 2025);
    if (ptr == NULL) {
        fprintf(stderr, "Function failed.\n");
        return EXIT_FAILURE;
    }

    // Check that the triple fields have proper values
    if (ptr->x != 1 || ptr->y != 13 || ptr->z != 2025) {
        fprintf(stderr, "Wrong value for point fields\n");
        return EXIT_FAILURE;
    }
    //  just for test
    //  printf("AllocatePoint3d() works fine: x = %u, y = %u, z = %u.\n", ptr->x, ptr->y, ptr->z);

    free(ptr);
    return EXIT_SUCCESS;
}

Point3dPtr AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z) {
    // malloc for a new Point3d
    // Point3dPtr is equivalent to Point3d*
    Point3dPtr retval = (Point3dPtr)malloc(sizeof(Point3d));
    // assign given value to the new Point3d if malloc success
    if (retval != NULL) {
        retval->x = x;
        retval->y = y;
        retval->z = z;
    }
    return retval;
}