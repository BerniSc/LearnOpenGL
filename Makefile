CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lglfw -ldl -lGL
INCLUDES = -I./include  -I./
TARGET = main

# Source files
SRCS = glad.o src/core/GLException.cpp src/core/GLWindow.cpp src/core/IndexBuffer.cpp src/core/Shader.cpp src/core/ShaderProgram.cpp src/core/Texture.cpp src/core/VertexArray.cpp src/core/VertexBuffer.cpp src/drawable/Rectangle.cpp src/drawable/TexturedRectangle.cpp src/drawable/Triangle.cpp main.cpp 
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
