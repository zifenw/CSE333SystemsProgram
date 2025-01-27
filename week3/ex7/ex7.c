/*
 * Copyright 2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 01/26/25
 */

#include <stdio.h>   // for input/output functions
#include <stdlib.h>  // for exit and memory allocation
#include <string.h>  // for string manipulation
#include <dirent.h>  // for working with directories
#include <fcntl.h>   // for file operations
#include <unistd.h>  // for low-level file I/O
#include <errno.h>   // to access error codes for system calls

#define BUFFER_SIZE 1024

// Prints the txt files in the directory.
void ReadAndPrint(char *directory, char *file);
// Helper function to print the txt file in the directory.
void PrintFile(char *full_path);


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "The program needs a directory name.\n");
        return EXIT_FAILURE;
    }

    DIR *dirp = opendir(argv[1]);
    if (dirp == NULL) {
        fprintf(stderr, "Open directory %s failed.\n", argv[1]);
        return EXIT_FAILURE;
    }

    while (1) {
        struct dirent *entry = readdir(dirp);
        if (entry == NULL) {
            break;
        }
        int filename_length = strlen(entry->d_name);
        if ((filename_length >= 4) &&
            (strcmp(entry->d_name + (filename_length - 4), ".txt") == 0)) {
            ReadAndPrint(argv[1], entry->d_name);
        }
    }
    // clean up and exit
    closedir(dirp);
    return EXIT_SUCCESS;
}

// Copy named file in named directory to stdout
void ReadAndPrint(char *directory, char *file) {
    if (directory == NULL) {
        fprintf(stderr, "diectory cannot find\n");
        exit(EXIT_FAILURE);
    }

    if (file == NULL) {
        fprintf(stderr, "file cannot find\n");
        exit(EXIT_FAILURE);
    }

    // Concatenate directory and file name.
    int pathlen = strlen(directory);
    int filelen = strlen(file);
    // +2 since / between dirctory and file and \0 at the end
    char *full_path = (char *) malloc(sizeof(char) * (pathlen + filelen + 2));
    if (full_path == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }
    snprintf(full_path, pathlen + filelen + 2, "%s/%s", directory, file);

    PrintFile(full_path);
    free(full_path);
}

void PrintFile(char *full_path) {
    if (full_path == NULL) {
        fprintf(stderr, "full_path cannot find\n");
        exit(EXIT_FAILURE);
    }
    int fd = open(full_path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "open file %s failed\n", full_path);
        exit(EXIT_FAILURE);
    }
    char buffer[BUFFER_SIZE];
    while (1) {
        int res = read(fd, buffer, BUFFER_SIZE);
        if (res == 0) {
            // end of file
            break;
        } else if ((res == -1) && (errno != EINTR)) {
            // other error
            fprintf(stderr, "Error reading file: %s\n", full_path);
        } else {
            if (fwrite(buffer, res, 1, stdout) != 1) {
                fprintf(stderr, "write failed\n");
            }
        }
    }
    close(fd);
}
