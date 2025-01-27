### Goals
Learn to use POSIX I/O routines to scan a file directory and read the contents of selected files. (These routines will be needed in later parts of the large project sequence. This exercise provides a chance to learn how to use them in a much simpler setting without additional complications of the larger project.)

### Description 
Write a C program that:
- accepts a directory name as a command-line argument (the directory name can be a simple name or a longer file path; it might, or might not, have a leading '/' at the beginning; and might, or might not, have a trailing '/' at the end)
- scans through the directory looking for filenames that end in the four characters .txt (you should not scan subdirectories, and you do not need to sort the directory entries; you may also assume that no subdirectory itself has a directory name ending in .txt)
- reads the contents of those files using the POSIX open / read / close functions, copying the contents to stdout (you may use C standard library functions (hint: fwrite) to write to stdout, but you must use open / read / close to read the files).  

Note: do not format, label, or otherwise add to, delete from, or modify the contents of files copied to stdout. Copy the bytes from the input file(s) exactly as they are with no changes.

### Header
`stdio.h`: For input/output functions like `fprintf`, `fwrite`.  
`stdlib.h`: For general utilities like `exit` and memory allocation.  
`string.h`: For string manipulation (e.g., `strlen`, `strcmp`).  
`dirent.h`: For working with directories (`opendir`, `readdir`).  
`fcntl.h`: For file operations (`open`).  
`unistd.h`: For low-level file I/O (`read`, `close`).  
`errno.h`: To access error codes for system calls.  

### struct dirent
```c
struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
}
```

```c
/*
 * ex7.c
 * Author: [Your Name]
 * Email: [Your UW Email Address]
 * Description: This program scans a specified directory for files ending in ".txt",
 * reads their contents using POSIX I/O functions, and writes them to stdout.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 1024

// Function to check if a file name ends with ".txt"
int ends_with_txt(const char *filename) {
    size_t len = strlen(filename);
    return len > 4 && strcmp(filename + len - 4, ".txt") == 0;
}

// Function to handle errors and exit gracefully
void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Validate command-line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the specified directory
    const char *directory_name = argv[1];
    DIR *dir = opendir(directory_name);
    if (!dir) {
        handle_error("Failed to open directory");
    }

    struct dirent *entry;

    // Read entries in the directory
    while ((entry = readdir(dir)) != NULL) {
        // Skip directories and non-.txt files
        if (entry->d_type == DT_DIR || !ends_with_txt(entry->d_name)) {
            continue;
        }

        // Construct the full file path
        char filepath[PATH_MAX];
        snprintf(filepath, PATH_MAX, "%s/%s", directory_name, entry->d_name);

        // Open the file for reading
        int fd = open(filepath, O_RDONLY);
        if (fd == -1) {
            fprintf(stderr, "Failed to open file: %s\n", filepath);
            continue;
        }

        // Read and write the file contents to stdout
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
            if (fwrite(buffer, 1, bytes_read, stdout) != (size_t)bytes_read) {
                fprintf(stderr, "Error writing to stdout\n");
                close(fd);
                closedir(dir);
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1) {
            fprintf(stderr, "Error reading file: %s\n", filepath);
        }

        // Close the file
        close(fd);
    }

    // Close the directory
    if (closedir(dir) == -1) {
        handle_error("Failed to close directory");
    }

    return EXIT_SUCCESS;
}

```
### errno != EINTR 的逻辑
如果 errno == EINTR，表示系统调用被信号中断。此时通常不算作真正的错误，需要重新调用。  
如果 errno != EINTR，表示发生了其他错误（如文件描述符无效、权限不足等），此时需要停止重试并处理错误。

### test
```
gcc -Wall -std=c17 -g -o ex7 ex7.c  
./ex7 test_dir
./ex7 ../../week2
cpplint ex7.c


ssh zifenw@attu.cs.washington.edu
valgrind --leak-check=full ./ex6

```