/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/28/25
 */

#ifndef CLIENTSIDE_H_
#define CLIENTSIDE_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>


// Resolves the given hostname and port into a socket address.
// Returns true on success, initializes ret_addr and ret_addrlen.
// Returns false on failure (e.g., invalid hostname or port).
bool myGetaddrinfo(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen);

// Establishes a TCP connection to the specified address and port.
// Returns true on success, sets ret_fd to the connected socket.
// Returns false on failure (e.g., connection refused or timeout).
bool myConnect(const struct sockaddr_storage &addr,
             const size_t &addrlen,
             int *ret_fd);

// Safely reads data from a file descriptor, handling partial reads.
// Returns bytes read, 0 on EOF, or -1 on fatal errors.
int myRead(int fd, unsigned char *buf, int readlen);

// Safely writes data to a file descriptor, handling partial writes.
// Returns bytes written, or -1 on fatal errors.
int myWrite(int fd, const unsigned char *buf, int writelen);

// Prints the correct usage of the program to stderr and exits.
// Called when the program is invoked with incorrect arguments.
void Usage(char *progname);

#endif  // CLIENTSIDE_H_
