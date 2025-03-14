/*
 * Copyright ©2025 Zifeng Wang
 * 2478280
 * zifenw@uw.edu
 * 02/28/25
 */

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#include "ClientSide.h"

int main(int argc, char* argv[]) {
  // Check for correct number of arguments
  if (argc != 4) {
    Usage(argv[0]);
  }

  char* hostname = argv[1];
  unsigned short port = static_cast<unsigned short>(atoi(argv[2]));
  const char* filename = argv[3];

  // Resolve hostname to address
  struct sockaddr_storage addr;
  size_t addrlen;
  if (!myGetaddrinfo(hostname, port, &addr, &addrlen)) {
    return EXIT_FAILURE;
  }

  // Establish connection to server
  int sockfd;
  if (!myConnect(addr, addrlen, &sockfd)) {
    return EXIT_FAILURE;
  }

  // Open the file to send
  int file_fd = open(filename, O_RDONLY);
  if (file_fd == -1) {
    std::cerr << "open: " << strerror(errno) << std::endl;
    close(sockfd);
    return EXIT_FAILURE;
  }

  // Read file and send data to server
  unsigned char buffer[4096];
  int bytes_read;
  while ((bytes_read = myRead(file_fd, buffer, sizeof(buffer))) > 0) {
    if (myWrite(sockfd, buffer, bytes_read) == -1) {
      std::cerr << "Failed to write data to server" << std::endl;
      close(file_fd);
      close(sockfd);
      return EXIT_FAILURE;
    }
  }

  if (bytes_read == -1) {
    std::cerr << "Failed to read data from file" << std::endl;
  }

  // Clean up
  close(file_fd);
  close(sockfd);
  return EXIT_SUCCESS;
}
