// main.cpp

#include <cmath>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "myWrappers/GLException.hpp"
#include "myWrappers/ShaderProgram.hpp"
#include "myWrappers/Triangle.hpp"
#include "myWrappers/Rectangle.hpp"
#include "myWrappers/GLWindow.hpp"

#include <iostream>
#include <ostream>
#include <string>

const char* vertexShaderTriangleSrc = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor;                   // specify a color that gets output#ed to the fragment shader -> Shader-Shader communication
void main() {
   gl_Position = vec4(aPos, 1.0);
   vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // set the output variable to a dark-red color
}
)glsl";

const char* vertexShaderTriangleColourSrc = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;   // color variable at ttribute position 1
out vec4 vertexColor;                   // specify a color that gets output#ed to the fragment shader -> Shader-Shader communication
void main() {
   gl_Position = vec4(aPos, 1.0);
   vertexColor = vec4(aColor, 1.0);
}
)glsl";

const char* fragmentShaderPassthrough = R"glsl(
#version 330 core
out vec4 FragColor;
in vec4 vertexColor;            // the input variable from the vertex shader (same name and same type) 
void main() {
    FragColor = vertexColor;
} 
)glsl";

const char* fragmentShaderOrangeSrc = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)glsl";

const char* fragmentShaderRedSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(1.0, 0.2, 0.1, 0.7);
}
)glsl";

const char* fragmentShaderGreenSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(0.1, 1.0, 0.4, 0.7);
}
)glsl";

const char* fragmentShaderBlueSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(0.1, 0.4, 1.0, 0.7);
}
)glsl";

int main() {
    try {
        GLWindow window(800, 800, "Clean Triangle");

        ShaderProgram shader(vertexShaderTriangleSrc, fragmentShaderOrangeSrc);
        ShaderProgram shaderRedTriangle(vertexShaderTriangleSrc, fragmentShaderRedSource);
        /*ShaderProgram shaderGreen(vertexShaderTriangleSrc, fragmentShaderGreenSource);*/
        ShaderProgram shaderGreen(true, "./shaders/triangle.vs.glsl", "./shaders/shaderUniformed.fs.glsl");
        shaderGreen.setFloat4("uniformedColor", 0.5f, 0.2f, 0.5f, 0.0f);
        ShaderProgram shaderBlue(vertexShaderTriangleSrc, fragmentShaderBlueSource);
        ShaderProgram passthrough(vertexShaderTriangleSrc, fragmentShaderPassthrough);
        ShaderProgram myLoadFile(true, "./shaders/triangle.vs.glsl", "./shaders/shaderRed.fs.glsl");

        ShaderProgram colourfull(vertexShaderTriangleColourSrc, fragmentShaderPassthrough);

        Triangle triangle(shader);

        // x-y-z regular coordinate-system
        static constexpr float verticesSmaller[9] = {
             -0.3f,  0.1f, 0.0f,
             0.0f,  -0.5f, 0.0f,
             0.3f,  0.1f, 0.0f
        };

        static constexpr float verticesRectSmaller[12] = {
             0.2f,  0.2f, 0.0f,  // top right
             0.2f, -0.2f, 0.0f,  // bottom right
            -0.2f, -0.2f, 0.0f,  // bottom left
            -0.2f,  0.2f, 0.0f   // top left
        };


        /*Triangle triangleSmaller(shaderRedTriangle, verticesSmaller);*/
        Triangle triangleSmaller(shaderGreen, verticesSmaller);

        /*Rectangle rectangle(shaderGreen);*/
        Rectangle rectangle(myLoadFile);
        /*Rectangle rectangleBlue(shaderBlue, verticesRectSmaller);*/
        Rectangle rectangleBlue(passthrough, verticesRectSmaller);

        while(window.shouldKeepAlive()) {
            window.clear();
            rectangle.draw();
            triangle.draw();
            triangleSmaller.draw();
            rectangleBlue.draw();
            window.display();
            window.pollEvents();
        }

    } catch(const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
