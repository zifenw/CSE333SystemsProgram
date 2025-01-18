/*
 * Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/17/25
 */
#include <stdio.h>
#include <stdlib.h>

// print error message and exit
void errorMessage(void) {
    fprintf(stderr, "Input error, should be ./ex6 filename\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    // Open the file given as program argument.
    // Exit is input error.
    if (argc != 2) {
        errorMessage();
    }
    // Exit if file not found
    // "rb" binary read only
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        errorMessage();
    }

    // get size
    long fileSize;
    // fseek(file, 0, SEEK_END) succeed when equal 0
    if (fseek(file, 0, SEEK_END) !=0) {
        errorMessage();
    }
    // ftell return the how many byte fileptr from the beginning
    // if fail return -1
    fileSize = ftell(file);
    if (fileSize == -1) {
        errorMessage();
    }
    // Iterate the file byte-by-byte from the end
    for (long i = fileSize - 1; i >= 0; i--) {
        char character;
        if (fseek(file, i, SEEK_SET) != 0) {
            errorMessage();
        }
        if (fread((void*) &character, sizeof(char), 1, file) != sizeof(char)) {
            errorMessage();
        }
        printf("%c", character);
    }

    fclose(file);
    return EXIT_SUCCESS;
}