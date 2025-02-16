CC = g++
CFLAGS = -std=c++11 -Wall

# Object files
OBJECTS = ex13.o Cartesian.o Polar.o

# Target executable
TARGET = ex13

# Make target to build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile ex13.o
ex13.o: ex13.cc Cartesian.h Polar.h
	$(CC) $(CFLAGS) -c ex13.cc

# Compile Cartesian.o
Cartesian.o: Cartesian.cc Cartesian.h
	$(CC) $(CFLAGS) -c Cartesian.cc

# Compile Polar.o
Polar.o: Polar.cc Polar.h
	$(CC) $(CFLAGS) -c Polar.cc

# Clean up
clean:
	rm -f $(TARGET) $(OBJECTS) *~