CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lglfw -ldl -lGL
INCLUDES = -I./include
TARGET = main

# Source files
SRCS = main.cpp glad.o
# Object files
OBJS = $(SRCS:.cpp=.o)

# Main target
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(INCLUDES) -o $(TARGET)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run:
	./$(TARGET)

# Create include directory if it doesn't exist
create_dirs:
	@mkdir -p include

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean create_dirs run
