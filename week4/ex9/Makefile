# Compiler
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

# Executable and source files
TARGET = ex9
SOURCES = Vector.cc ex9.cc
OBJECTS = $(SOURCES:.cc=.o)

# Default target: build the executable
all: $(TARGET)

# Build the executable by linking object files
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cc files into .o files
%.o: %.cc Vector.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove generated files
clean:
	rm -f $(TARGET) $(OBJECTS) *~