/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 03/02/25
 */
#include "ServerSide.h"  // Include the SocketUtil header
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstring>

#define BUFSIZE 1024
using std::cerr;
using std::endl;
using std::cout;

// Prints usage information about any failure
void Usage(const char* progname);

int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        Usage(argv[0]);
    }

    // Extract the port number from the command line arguments
    char* portnum = argv[1];
    int sock_family;

    // Create a listening socket on the specified port
    int listen_fd = myListen(portnum, &sock_family);
    if (listen_fd == -1) {
        cerr << "Failed to create listening socket." << endl;
        Usage(argv[0]);
    }

    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    int client_fd = -1;

    // Accepting a connection from a client
    while (true) {
        client_fd = accept(listen_fd, (struct sockaddr*)&caddr, &caddr_len);
        if (client_fd < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            cerr << "Error on accept: " << strerror(errno) << endl;
            close(listen_fd);
            return EXIT_FAILURE;
        }
        break;
    }


    // Read data from the client and write it to stdout
    unsigned char buf[BUFSIZE];
    while (true) {
        int res = myRead(client_fd, buf, BUFSIZE);
        if (res == 0) {
            cout << "The client disconnected." << endl;
            break;
        }
        if (res == -1) {
            cerr << "Error reading from client socket." << endl;
            break;
        }

        // Write the received data to stdout
        if (fwrite(buf, 1, res, stdout) != static_cast<size_t>(res)) {
            cerr << "Error writing to stdout." << endl;
            break;
        }
    }

    close(client_fd);
    close(listen_fd);
    return EXIT_SUCCESS;
}

void Usage(const char* progname) {
  cerr << "Usage: " << progname << " <port>" << endl;
  exit(EXIT_FAILURE);
}
