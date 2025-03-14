/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 03/02/25
 */
#ifndef SERVERSIDE_H_
#define SERVERSIDE_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

// Creates a listening socket on the specified port and returns the socket
// descriptor. On success, returns the socket file descriptor; on failure,
// returns -1.
int myListen(char* portnum, int* sock_family);

// Reads data from the file descriptor into the buffer, handling partial
// reads and errors. Returns the number of bytes read, or -1 on fatal error.
int myRead(int fd, unsigned char* buf, int readlen);

// Writes data from the buffer to the file descriptor, handling partial
// writes and errors. Returns the number of bytes written, or -1 on fatal
// error.
int myWrite(int fd, unsigned char* buf, int writelen);

#endif  // SERVERSIDE_H_
