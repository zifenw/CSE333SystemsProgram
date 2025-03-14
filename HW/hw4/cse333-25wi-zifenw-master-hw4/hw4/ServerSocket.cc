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

#include <stdio.h>       // for snprintf()
#include <unistd.h>      // for close(), fcntl()
#include <sys/types.h>   // for socket(), getaddrinfo(), etc.
#include <sys/socket.h>  // for socket(), getaddrinfo(), etc.
#include <arpa/inet.h>   // for inet_ntop()
#include <netdb.h>       // for getaddrinfo()
#include <errno.h>       // for errno, used by strerror()
#include <string.h>      // for memset, strerror()
#include <iostream>      // for std::cerr, etc.

#include "./ServerSocket.h"

extern "C" {
  #include "libhw1/CSE333.h"
}

using std::string;
using std::to_string;
using std::cerr;
using std::endl;

namespace hw4 {

ServerSocket::ServerSocket(uint16_t port) {
  port_ = port;
  listen_sock_fd_ = -1;
}

ServerSocket::~ServerSocket() {
  // Close the listening socket if it's not zero.  The rest of this
  // class will make sure to zero out the socket if it is closed
  // elsewhere.
  if (listen_sock_fd_ != -1)
    close(listen_sock_fd_);
  listen_sock_fd_ = -1;
}

bool ServerSocket::BindAndListen(int ai_family, int *const listen_fd) {
  // Use "getaddrinfo," "socket," "bind," and "listen" to
  // create a listening socket on port port_.  Return the
  // listening socket through the output parameter "listen_fd"
  // and set the ServerSocket data member "listen_sock_fd_"

  // STEP 1:
  // Setup address info hints structure for getaddrinfo()
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = ai_family;      // Allow IPv4/IPv6 based on input
  hints.ai_socktype = SOCK_STREAM;  // Stream socket (TCP)
  hints.ai_flags = AI_PASSIVE;      // Wildcard address for server
  hints.ai_flags |= AI_V4MAPPED;    // Allow IPv4-mapped IPv6 if no v6 found
  hints.ai_protocol = IPPROTO_TCP;  // Use TCP protocol

  // Convert port number to string for getaddrinfo()
  string port_str = to_string(this->port_);
  struct addrinfo* addr_list;
  int status = getaddrinfo(nullptr, port_str.c_str(), &hints, &addr_list);

  if (status != 0) {
    cerr << "getaddrinfo() failed: ";
    cerr << gai_strerror(status) << endl;
    return false;
  }

  // Iterate through address results to create, bind, and listen on a socket
  int server_fd = -1;
  struct addrinfo* addr;
  for (addr = addr_list; addr != nullptr; addr = addr->ai_next) {
    // Create socket
    server_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);

    if (server_fd == -1) {
      cerr << "socket() failed: " << strerror(errno) << endl;
      continue;  // Try next address in the list
    }

    // Set SO_REUSEADDR to allow quick reuse of the port after program restart
    int optval = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval,
               sizeof(optval));

    // Bind the socket to the address and port
    if (bind(server_fd, addr->ai_addr, addr->ai_addrlen) == 0) {
      this->sock_family_ = addr->ai_family;
      cerr << "bind() failed: " << strerror(errno) << endl;
      break;  // Successfully bound, exit loop
    }

    // If bind failed, close the socket and try the next address
    cerr << "bind() failed: " << strerror(errno) << endl;
    close(server_fd);
    server_fd = -1;
  }

  // Free address list
  freeaddrinfo(addr_list);

  // If binding failed for all addresses, return failure
  if (server_fd == -1) {
    cerr << "Failed to bind to any address." << endl;
    return false;
  }

  // Set the socket to listen mode
  if (listen(server_fd, SOMAXCONN) != 0) {
    cerr << "Failed to mark socket as listening: "
    << strerror(errno) << endl;
    close(server_fd);
    return false;
  }

  // Store the listening socket file descriptor
  this->listen_sock_fd_ = server_fd;
  *listen_fd = server_fd;


  return true;
}

bool ServerSocket::Accept(int *const accepted_fd,
                          string *const client_addr,
                          uint16_t *const client_port,
                          string *const client_dns_name,
                          string *const server_addr,
                          string *const server_dns_name) const {
  // STEP 2:
  // Accept a new connection on the listening socket.
  // This blocks until a client connects.

  int client_fd;  // File descriptor for the accepted client connection
  struct sockaddr_storage client_sockaddr;  // Stores client address
  socklen_t client_addr_len = sizeof(client_sockaddr);

  // Loop to handle interruptions and retry accepting a connection.
  while (true) {
    client_fd = accept(this->listen_sock_fd_,
                       reinterpret_cast<struct sockaddr*>(&client_sockaddr),
                       &client_addr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR)) {
        continue;  // Retry on temporary errors
      }
      cerr << "Error: accept() failed: " << strerror(errno) << endl;
      close(this->listen_sock_fd_);
      return false;
    }

    break;  // Successfully accepted a client connection
  }

  // Update the client socket to accept one
  *accepted_fd = client_fd;

  // Extract client IP address and port
  char client_ip_str[INET6_ADDRSTRLEN];  // Buffer for IP address
  if (client_sockaddr.ss_family == AF_INET) {
    struct sockaddr_in* client_ipv4 =
      reinterpret_cast<struct sockaddr_in*>(&client_sockaddr);

    inet_ntop(AF_INET, &(client_ipv4->sin_addr),
      client_ip_str, INET_ADDRSTRLEN);

    *client_addr = std::string(client_ip_str);

    // Convert port to host byte order
    *client_port = ntohs(client_ipv4->sin_port);

  } else {
    struct sockaddr_in6* client_ipv6 =
      reinterpret_cast<struct sockaddr_in6*>(&client_sockaddr);

    inet_ntop(AF_INET6, &(client_ipv6->sin6_addr),
      client_ip_str, INET6_ADDRSTRLEN);

    *client_addr = std::string(client_ip_str);

    *client_port = ntohs(client_ipv6->sin6_port);
  }

  // Retrieve client's DNS name
  char client_hostname[NI_MAXHOST] = {0};  // Buffer for client hostname
  if (getnameinfo(reinterpret_cast<struct sockaddr*>(&client_sockaddr),
                  client_addr_len, client_hostname, NI_MAXHOST,
                  nullptr, 0, 0) != 0) {
    snprintf(client_hostname, NI_MAXHOST, "[reverse DNS lookup failed]");
  }
  *client_dns_name = std::string(client_hostname);

  // Retrieve server's IP and DNS name
  char server_hostname[NI_MAXHOST] = {0};  // Buffer for server hostname
  char server_ip_str[INET6_ADDRSTRLEN];  // Buffer for server IP address
  if (sock_family_ == AF_INET) {
    struct sockaddr_in server_sockaddr;

    socklen_t server_addr_len = sizeof(server_sockaddr);

    getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&server_sockaddr),
                &server_addr_len);

    inet_ntop(AF_INET, &server_sockaddr.sin_addr,
              server_ip_str, INET_ADDRSTRLEN);

    getnameinfo(reinterpret_cast<struct sockaddr*>(&server_sockaddr),
                server_addr_len, server_hostname, NI_MAXHOST, nullptr, 0, 0);

  } else {
    struct sockaddr_in6 server_sockaddr;

    socklen_t server_addr_len = sizeof(server_sockaddr);

    getsockname(client_fd, reinterpret_cast<struct sockaddr*>(&server_sockaddr),
                &server_addr_len);

    inet_ntop(AF_INET6, &server_sockaddr.sin6_addr,
              server_ip_str, INET6_ADDRSTRLEN);

    getnameinfo(reinterpret_cast<struct sockaddr*>(&server_sockaddr),
                server_addr_len, server_hostname, NI_MAXHOST, nullptr, 0, 0);
  }

  // Store server details
  *server_addr = std::string(server_ip_str);
  *server_dns_name = std::string(server_hostname);

  return true;
}

}  // namespace hw4
