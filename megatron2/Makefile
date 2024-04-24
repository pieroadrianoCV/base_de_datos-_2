CXX = g++
CXXFLAGS = -Wall -g -std=c++11  # Add the C++11 standard and other common flags

# Name of the executable
TARGET = MEGATRON

# All object files that will be generated
OBJS = main.o database.o

# Default rule
all: $(TARGET)

# How to link the program
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# How to compile each source file
main.o: main.cpp database.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

megatron.o: database.cpp database.h
	$(CXX) $(CXXFLAGS) -c database.cpp -o database.o
# Command to clean compiled files (optional)
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
