#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/GLException.hpp"
#include "core/GLWindow.hpp"
#include "core/Shader.hpp"
#include "core/ShaderProgram.hpp"
#include "drawable/TexturedRectangle.hpp"
#include "drawable/Triangle.hpp"
#include "drawable/Rectangle.hpp"

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
        GLWindow window(800, 800, "Refactored OpenGL");

        // Load shaders from files
        Shader vertexShader(GL_VERTEX_SHADER, "./shaders/simpleVertexShader.vs.glsl", true);          // true means load from file
        Shader fragmentShader(GL_FRAGMENT_SHADER, "./shaders/triangle.fs.glsl", true);                // true means load from file

        Shader colourfullVertexShader(GL_VERTEX_SHADER, "./shaders/triangle.vs.glsl", true);
        Shader colourfullFragmentShader(GL_FRAGMENT_SHADER, "./shaders/shaderPipedColour.fs.glsl", true);

        Shader uniformedVectexShader(GL_VERTEX_SHADER, "./shaders/simpleVertexShader.vs.glsl", true);
        Shader uniformedFragmentShader(GL_FRAGMENT_SHADER, "./shaders/shaderUniformed.fs.glsl", true);
        
        // Shader Programs
        ShaderProgram shader(vertexShader, fragmentShader);
        ShaderProgram breathing(uniformedVectexShader, uniformedFragmentShader);
        ShaderProgram colourfullProg(colourfullVertexShader, colourfullFragmentShader); 
        /*breathing.setFloat4("uniformedColor", 0.0f, 0.8f, 0.0f, 0.0f);*/

        Shader taskTestPipeV(GL_VERTEX_SHADER, "./shaders/shaderPipedColour.vs.glsl", true);
        Shader taskTestPipeF(GL_FRAGMENT_SHADER, "./shaders/shaderPipedColour.fs.glsl", true);
        ShaderProgram testTest(taskTestPipeV, taskTestPipeF);

        // Vertices for shapes
        static constexpr float triangleVertices[9] = {
            -0.5f, -0.3f, 0.0f, 
             0.5f, -0.3f, 0.0f, 
             0.0f,  0.8f, 0.0f  
        };

        static constexpr float triangleReversedVerticesWithColour[18] = {
            -0.55f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.55f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.0f,  -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        };
        std::vector<VertexAttribute> attributesVertexColour = {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float)}
        };

        std::vector<VertexAttribute> textureAttributes = {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)}
        };

        static constexpr float rectangleVertices[12] = {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
           -0.5f, -0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f
        };

        // Create drawable objects
        /*Triangle triangle(shader, triangleVertices, true);*/
        Triangle triangle(testTest, triangleVertices);
        Triangle triangleRevered(colourfullProg, triangleReversedVerticesWithColour, 3, 6 * sizeof(float), attributesVertexColour, false);
        Rectangle rectangle(breathing, rectangleVertices);

        float timeStamp = glfwGetTime();
        float breathingGreenValue = 0.0f;
        float breathingBlueValue = 0.0f;
        float breathingRedValue = 0.0f;

        Shader simpleTextureVS(GL_VERTEX_SHADER, "./shaders/simpleTexture.vs.glsl", true);
        Shader simpleTextureFS(GL_FRAGMENT_SHADER, "./shaders/simpleTexture.fs.glsl", true);
        ShaderProgram simpleTexture(simpleTextureVS, simpleTextureFS);
        TexturedRectangle trect(simpleTexture, "./assets/wall.jpg", 4, 5 * sizeof(float), textureAttributes);

        // Main render loop
        while(window.shouldKeepAlive()) {
            window.clear();
            rectangle.draw();
            triangleRevered.draw();
            triangle.draw();
            trect.draw();
            window.display();
            window.pollEvents();

            // Recalculate Breathing
            breathing.setFloat4("uniformedColor", breathingRedValue, breathingGreenValue, breathingBlueValue, 1.0f);
            breathingGreenValue = (std::sin(timeStamp / 2) / 2.0f) + 0.5f;
            breathingBlueValue = (std::sin(timeStamp / 3 + M_2_PI) / 2.0f) + 0.5f;
            breathingRedValue = (std::sin(timeStamp / 4) / 2.0f) + 0.5f;
            timeStamp = glfwGetTime();
        }

    } catch(const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

