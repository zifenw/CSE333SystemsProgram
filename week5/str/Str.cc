// CSE 333 Lecture 12 demo: Str.cc
// Hal Perkins

// Implementation of a simple string class.

#include <cstring>
#include <iostream>
#include "Str.h"

using namespace std;

// Print trace messages with object addresses to show when functions get called.
#define TRACE(what)  cout << "- Str::" what << " called on " << this << endl

// constructors

Str::Str() {
  TRACE("default constructor");
  // allocate empty string
  st_ = new char[1];
  st_[0] = '\0';
}

Str::Str(const char *s) {
  TRACE("c-string constructor");
  int len = strlen(s);
  st_ = new char[len+1];
  strcpy(st_, s);
}

Str::Str(const Str &s) {
  TRACE("copy constructor");
  int len = strlen(s.st_);
  st_ = new char[len+1];
  strcpy(st_, s.st_);
}

// destructor
Str::~Str() {
  TRACE("destructor");
  delete [] st_;
}

// operations

int Str::length() const {
  TRACE("length");
  return strlen(st_);
}

char * Str::c_str() const {
  TRACE("c_str");
  char * result = new char[strlen(st_)+1];
  strcpy(result, st_);
  return result;
}


void Str::append(const Str &s) {
  TRACE("append");
  char *newst = new char[strlen(st_) + strlen(s.st_) + 1];
  strcpy(newst, st_);
  strcat(newst, s.st_);
  delete [] st_;
  st_ = newst;
}

Str & Str::operator=(const Str &s) {
  TRACE("assignment");
  // do nothing if trying to assign a string to itself
  if (this == &s) {
    return *this;
  }

  // Replace string array with one of correct size
  // (Note: this is where the self-assignment test _really_ matters)
  // (Note: a better implementation would re-use the existing array
  //  if it's long enough, but not too long)
  delete [] st_;
  st_ = new char[strlen(s.st_)+1];

  // copy characters and return
  strcpy(st_, s.st_);
  return *this;
}

// stream output
// (Note: not a member function of Str.  This is an overloaded global
// function that is a friend of Str so it can access private data)
ostream &operator<<(ostream &out, const Str &s) {
  out << s.st_;
  return out;
}