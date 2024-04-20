CXX = g++
CXXFLAGS = -Wall -g -std=c++11  # Add the C++11 standard and other common flags

# Name of the executable
TARGET = MEGATRON

# All object files that will be generated
OBJS = main.o megatron.o helpers.o

# Default rule
all: $(TARGET)

# How to link the program
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# How to compile each source file
main.o: main.cpp megatron.h helpers.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

megatron.o: megatron.cpp megatron.h helpers.h
	$(CXX) $(CXXFLAGS) -c megatron.cpp -o megatron.o

helpers.o: helpers.cpp helpers.h
	$(CXX) $(CXXFLAGS) -c helpers.cpp -o helpers.o

# Command to clean compiled files (optional)
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
