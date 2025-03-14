/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/28/25
 */

#include <unistd.h>
#include <string.h>
#include <iostream>

#include "ClientSide.h"

using std::cerr;
using std::endl;

bool myGetaddrinfo(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Do the lookup by invoking getaddrinfo().
    int retval = getaddrinfo(name, nullptr, &hints, &res);
    if (retval != 0) {
        cerr << "getaddrinfo failed: ";
        cerr << gai_strerror(retval) << endl;
        return false;
    }

    // Set the port in the first result.
    if (res->ai_family == AF_INET) {
        struct sockaddr_in *v4addr = (struct sockaddr_in *) res->ai_addr;
        v4addr->sin_port = htons(port);
    } else if (res->ai_family == AF_INET6) {
        struct sockaddr_in6 *v6addr = (struct sockaddr_in6 *) res->ai_addr;
        v6addr->sin6_port = htons(port);
    } else {
        std::cerr << "getaddrinfo failed to provide an IPv4 or IPv6 address";
        std::cerr << std::endl;
        freeaddrinfo(res);
        return false;
    }

    // Return the first result.
    memcpy(ret_addr, res->ai_addr, res->ai_addrlen);
    *ret_addrlen = res->ai_addrlen;

    // Clean up.
    freeaddrinfo(res);
    return true;
}

// Establishes a TCP connection to the specified address and port.
// Returns true on success, sets ret_fd to the connected socket.
// Returns false on failure (e.g., connection refused or timeout).
bool myConnect(const struct sockaddr_storage &addr,
             const size_t &addrlen,
             int *ret_fd) {
    // Create a TCP socket
    int fd = socket(addr.ss_family, SOCK_STREAM, 0);
    if (fd == -1) {
        cerr << "Error creating socket" << endl;
        return false;
    }
    // Connect to the server
    if (connect(fd, reinterpret_cast<const sockaddr*>(&addr)
        , addrlen) == -1) {
        cerr << "Error connecting to server" << endl;
        close(fd);
        return false;
    }
    *ret_fd = fd;
    return true;
}

int myRead(int fd, unsigned char *buf, int readlen) {
    int bytes_read = 0;
    while (bytes_read < readlen) {
        int result = read(fd, buf + bytes_read, readlen - bytes_read);
        if (result == -1) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            } else {
                cerr << "read: " << strerror(errno) << endl;
                return -1;
            }
        } else if (result == 0) {
            break;  // EOF
        }
        bytes_read += result;
    }
    return bytes_read;
}

int myWrite(int fd, const unsigned char *buf, int writelen) {
    int bytes_written = 0;
    while (bytes_written < writelen) {
        int result = write(fd, buf + bytes_written, writelen - bytes_written);
        if (result == -1) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            } else {
                cerr << "write: " << strerror(errno) << endl;
                return -1;
            }
        }
        bytes_written += result;
    }
    return bytes_written;
}

void Usage(char *progname) {
  cerr << "Usage: " << progname << " <hostname> <port> <filename>" << endl;
  exit(EXIT_FAILURE);
}
