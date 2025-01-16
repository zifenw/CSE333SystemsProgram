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

#include <stdio.h>    // for printf
#include <stdlib.h>   // for EXIT_SUCCESS, malloc, free
#include <stdint.h>   // for uint16_t

typedef struct point3d {  // a 3-D point:
  uint16_t x, y, z;       //   x, y, and z coordinates
} Point3d;

// Allocate and return a pointer to a new Point3d in the heap
// with fields initialized to the given x, y, and z values.
Point3d *AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z);

int main(int argc, char **argv) {
  // Try to allocate a triple.
  Point3d *pt = AllocatePoint3d(3, 5, 333);
  if (pt == NULL) {
    fprintf(stderr, "AllocatePoint3d() failed.\n");
    return EXIT_FAILURE;
  }

  // Check that the triple fields have proper values
  if (pt->x != 3 || pt->y != 5 || pt->z != 333) {
    fprintf(stderr, "One or more point fields have wrong value\n");
    free(pt);
    return EXIT_FAILURE;
  }

  // It worked!  Clean up by freeing the allocated memory.
  free(pt);
  return EXIT_SUCCESS;
}

Point3d *AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z) {
  Point3d *retval = (Point3d *) malloc(sizeof(Point3d));
  if (retval == NULL) {
    // out of memory
    return NULL;
  }

  // Store x,y,z values in new point's fields
  retval->x = x;
  retval->y = y;
  retval->z = z;

  // Return (a pointer to) the newly allocated 3-D point
  return retval;
}