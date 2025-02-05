# Copyright 2025 Zifeng Wang
# 2478280
# zifenw@uw.edu
# 02/03/31
# Makefile

CXX = g++
OBJS = Vector.o ex10.o
EXE  = ex10
HEADERS = Vector.h
CPPFLAGS = -Wall -g -std=c++17

# default target
$(EXE): $(OBJS)
	$(CXX) $(CPPFLAGS) -o $(EXE) $(OBJS)

# source files
%.o: %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $<

# phony target - remove generated files and backups
clean:
	rm -rf $(EXE) *.o *~ *.dSYM