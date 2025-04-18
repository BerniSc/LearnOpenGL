#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/GLWindow.hpp"
#include "core/Shader.hpp"
#include "core/ShaderProgram.hpp"
#include "drawable/Triangle.hpp"
#include "drawable/Rectangle.hpp"
#include "myWrappers/GLException.hpp"

const char* vertexShaderSrc = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0); 
}
)glsl";

const char* fragmentShaderSrc = R"glsl(
#version 330 core
out vec4 FragColor;
in vec4 vertexColor;
void main() {
    FragColor = vertexColor;
}
)glsl";

int main() {
    try {
        GLWindow window(800, 600, "Refactored OpenGL");

        // Load shaders from files
        Shader vertexShader(GL_VERTEX_SHADER, "./shaders/triangle.vs.glsl", true);          // true means load from file
        std::cout << "Still here";
        Shader fragmentShader(GL_FRAGMENT_SHADER, "./shaders/triangle.fs.glsl", true);      // true means load from file
        // Shader Programs
        ShaderProgram shader(vertexShader, fragmentShader);

        // Vertices for shapes
        static constexpr float triangleVertices[9] = {
            -0.5f, -0.5f, 0.0f, 
             0.5f, -0.5f, 0.0f, 
             0.0f,  0.5f, 0.0f  
        };
        static constexpr float rectangleVertices[12] = {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
           -0.5f, -0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f
        };

        // Create drawable objects
        Triangle triangle(shader, triangleVertices);
        Rectangle rectangle(shader, rectangleVertices);

        // Main render loop
        while(window.shouldKeepAlive()) {
            window.clear();
            /*triangle.draw();*/
            rectangle.draw();
            window.display();
            window.pollEvents();
        }

    } catch (const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

