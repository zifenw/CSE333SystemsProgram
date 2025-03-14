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

#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <vector>

#include "./HttpRequest.h"
#include "./HttpUtils.h"
#include "./HttpConnection.h"
#define BUFSIZE 4096

using std::map;
using std::string;
using std::vector;

namespace hw4 {

static const char *kHeaderEnd = "\r\n\r\n";
static const int kHeaderEndLen = 4;

bool HttpConnection::GetNextRequest(HttpRequest *const request) {
  // Use WrappedRead from HttpUtils.cc to read bytes from the files into
  // private buffer_ variable. Keep reading until:
  // 1. The connection drops
  // 2. You see a "\r\n\r\n" indicating the end of the request header.
  //
  // Hint: Try and read in a large amount of bytes each time you call
  // WrappedRead.
  //
  // After reading complete request header, use ParseRequest() to parse into
  // an HttpRequest and save to the output parameter request.
  //
  // Important note: Clients may send back-to-back requests on the same socket.
  // This means WrappedRead may also end up reading more than one request.
  // Make sure to save anything you read after "\r\n\r\n" in buffer_ for the
  // next time the caller invokes GetNextRequest()!

  // STEP 1:
  size_t pos = buffer_.find(kHeaderEnd);
  if (pos == std::string::npos) {
    char temp_buf[BUFSIZE];  // Temporary buffer for reading data
    while (1) {
      int bytes_read = WrappedRead(fd_,
                                  reinterpret_cast<unsigned char*>(temp_buf),
                                  BUFSIZE);

      if (bytes_read == 0) {
        // EOF or connection dropped
        break;
      } else if (bytes_read == -1) {
        // read failed
        return false;
      } else {
        buffer_ += string(temp_buf, bytes_read);  // Append new data

        // stop reading if read kHeaderEnd
        pos = buffer_.find(kHeaderEnd);
        if (pos != std::string::npos)
          break;
      }
    }
  }


  // Extract header portion from buffer_
  if (pos == std::string::npos)
    return false;

  string header = buffer_.substr(0, pos);
  *request = ParseRequest(header);

  // Preserve everything (if any) after "\r\n\r\n" in buffer_
  buffer_ = buffer_.substr(pos + kHeaderEndLen);
  return true;
}

bool HttpConnection::WriteResponse(const HttpResponse &response) const {
  // We use a reinterpret_cast<> to cast between unrelated pointer types, and
  // a static_cast<> to perform a conversion from an unsigned type to its
  // corresponding signed one.
  string str = response.GenerateResponseString();
  int res = WrappedWrite(fd_,
                         reinterpret_cast<const unsigned char*>(str.c_str()),
                         str.length());

  if (res != static_cast<int>(str.length()))
    return false;
  return true;
}

HttpRequest HttpConnection::ParseRequest(const string &request) const {
  HttpRequest req("/");  // by default, get "/".

  // Plan for STEP 2:
  // 1. Split the request into different lines (split on "\r\n").
  // 2. Extract the URI from the first line and store it in req.URI.
  // 3. For the rest of the lines in the request, track the header name and
  //    value and store them in req.headers_ (e.g. HttpRequest::AddHeader).
  //
  // Hint: Take a look at HttpRequest.h for details about the HTTP header
  // format that you need to parse.
  //
  // You'll probably want to look up boost functions for:
  // - Splitting a string into lines on a "\r\n" delimiter
  // - Trimming whitespace from the end of a string
  // - Converting a string to lowercase.
  //
  // Note: If a header is malformed, skip that line.

  // STEP 2:
  vector<string> request_lines;
  boost::split(request_lines, request, boost::is_any_of("\r\n"),
               boost::token_compress_on);

  // Trim leading and trailing whitespace from each line
  for (auto& line : request_lines) {
    boost::trim(line);
  }

  // Extract the URI from the first line (request method, URI, version)
  if (!request_lines.empty()) {
    vector<string> first_line_parts;
    boost::split(first_line_parts, request_lines[0], boost::is_any_of(" "),
                 boost::token_compress_on);
    if (first_line_parts.size() >= 2) {
      req.set_uri(first_line_parts[1]);  // Set URI from the request line
    }
  }

  // Parse headers from remaining lines
  for (size_t i = 1; i < request_lines.size(); i++) {
    vector<string> header_parts;
    boost::split(header_parts, request_lines[i], boost::is_any_of(": "),
                 boost::token_compress_on);

    if (header_parts.size() >= 2) {  // Ensure valid header format
      boost::to_lower(header_parts[0]);  // Convert header name to lowercase
      req.AddHeader(header_parts[0], header_parts[1]);  // Store header
    }
  }

  return req;
}

}  // namespace hw4
