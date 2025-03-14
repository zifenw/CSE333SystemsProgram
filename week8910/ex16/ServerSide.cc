/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 03/02/25
 */

#include "ServerSide.h"
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cstring>

using std::cerr;
using std::endl;

int myListen(char* portnum, int* sock_family) {
  // Initialize hints structure for getaddrinfo
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;       // Use IPv6
  hints.ai_socktype = SOCK_STREAM;  // Use TCP
  hints.ai_flags = AI_PASSIVE;      // Use wildcard address
  hints.ai_flags |= AI_V4MAPPED;    // Map IPv4 to IPv6
  hints.ai_protocol = IPPROTO_TCP;  // Use TCP protocol
  hints.ai_canonname = nullptr;     // No canonical name
  hints.ai_addr = nullptr;          // No address
  hints.ai_next = nullptr;          // No next structure

  // Get address information for the given port
  struct addrinfo* result;
  int res = getaddrinfo(nullptr, portnum, &hints, &result);
  if (res != 0) {
    cerr << "getaddrinfo() failed: " << gai_strerror(res) << endl;
    return -1;
  }

  // Loop through the returned address structures to find a bindable address
  int listen_fd = -1;
  for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
    // Create a socket for the current address structure
    listen_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (listen_fd == -1) {
      cerr << "socket() failed: " << strerror(errno) << endl;
      continue;  // Try the next address
    }

    // Configure the socket to reuse the address
    int optval = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Attempt to bind the socket to the address and port
    if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
      *sock_family = rp->ai_family;  // Set the socket family
      break;  // Successfully bound, exit the loop
    }

    // Binding failed, close the socket and try the next address
    close(listen_fd);
    listen_fd = -1;
  }

  // Free the address information structure
  freeaddrinfo(result);

  // If no address could be bound, return failure
  if (listen_fd <= 0) {
    cerr << "Couldn't bind to any addresses." << endl;
    return listen_fd;
  }

  // Mark the socket as a listening socket
  if (listen(listen_fd, SOMAXCONN) != 0) {
    cerr << "Failed to mark socket as listening: ";
    cerr << strerror(errno) << endl;
    close(listen_fd);
    return -1;
  }

  // Return the listening file descriptor
  return listen_fd;
}

int myRead(int fd, unsigned char* buf, int readlen) {
  int bytesRead = read(fd, buf, readlen);  // Attempt to read data
  while (1) {
    if (bytesRead == -1) {
      cerr << "Error reading from socket" << endl;
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;  // Retry on non-fatal errors
      }
    }
    break;  // Exit loop on success or fatal error
  }
  return bytesRead;  // Return the number of bytes read
}

int myWrite(int fd, unsigned char* buf, int writelen) {
  int totalBytesWritten = 0;  // Track total bytes written
  while (totalBytesWritten < writelen) {
    // Attempt to write data
    int bytesWritten = write(fd, buf + totalBytesWritten,
                            writelen - totalBytesWritten);
    if (bytesWritten == -1) {
      // Handle errors
      if (errno == EINTR) {
        // Interrupted system call, retry
        continue;
      } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
        // Resource temporarily unavailable, retry
        continue;
      } else {
        // Fatal error
        cerr << "Error writing to socket: " << strerror(errno) << endl;
        return -1;
      }
    } else if (bytesWritten == 0) {
      // No data written (unlikely for sockets)
      cerr << "Write returned 0 bytes." << endl;
      return totalBytesWritten;
    }

    // Update total bytes written
    totalBytesWritten += bytesWritten;
  }

  // Return total bytes written
  return totalBytesWritten;
}
