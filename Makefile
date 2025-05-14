CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -lglfw -ldl -lGL
LDFLAGS = -lglfw -ldl -lGL
INCLUDES = -I./include  -I./
TARGET = main

# Source files
ENGINE_SRCS = glad.o src/core/GLException.cpp src/core/GLWindow.cpp src/core/IndexBuffer.cpp src/core/Shader.cpp src/core/ShaderProgram.cpp src/core/Texture.cpp src/core/VertexArray.cpp src/core/VertexBuffer.cpp src/drawable/Rectangle.cpp src/drawable/TexturedRectangle.cpp src/drawable/TexturedCube.cpp src/drawable/Triangle.cpp src/core/Camera.cpp src/config/CameraConfig.cpp 
# Object files
ENGINE_OBJS = $(ENGINE_SRCS:.cpp=.o)

FRACTAL_MAIN = fractal/newton.cpp
GAME_MAIN = games/3DSnake/main.cpp

# Main target
$(TARGET): $(ENGINE_OBJS) main.o
	$(CXX) $(ENGINE_OBJS) main.cpp $(CXXFLAGS) $(INCLUDES) -o $(TARGET)

fractal: $(ENGINE_OBJS) fractal/newton.o
	$(CXX) $(ENGINE_OBJS) fractal/newton.o $(INCLUDES) -o $(TARGET) $(LDFLAGS)
	
snake: $(ENGINE_OBJS) games/3DSnake/main.o
	$(CXX) $(ENGINE_OBJS) games/3DSnake/main.o $(INCLUDES) -o $(TARGET) $(LDFLAGS)

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

.PHONY: clean create_dirs run fractal snake
