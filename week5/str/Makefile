# Makefile
# CSE 333 Lecture 12 code

CXX = g++
CPPFLAGS = -Wall -g -std=c++17
PROGS =  strtest

# default target attempts to build all executables
all: $(PROGS)

# Str class example - with new/delete
strtest: strtest.o Str.o
	$(CXX) $(CPPFLAGS) -o $@ $^

strtest.o: strtest.cc Str.h
	$(CXX) $(CPPFLAGS) -c $<

Str.o: Str.cc Str.h
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf *.o *~ *.dSYM $(PROGS)