#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "myWrappers/ShaderProgram.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <GL/gl.h>

class Rectangle {
    private:
        GLuint elementBufferObjectID = 0;
        GLuint vertexBufferArrayID = 0;
        GLuint vertexBufferObjectID = 0;
        const ShaderProgram& shader;

        const float* vertices;

        // Our recangle consists of two triangles as OpenGL is designed to work with those
        // As we do not want to specify unneccesary points and send them to the GPU as this 
        // is expensive we use this approach with 4 points and then indices 
        // We 
        // Fallback vertices
        static constexpr float vertices_[12] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };

        unsigned int indices[6] = {  
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        void initialize();

    public:
        Rectangle(const ShaderProgram& shader, const float* customVertices = vertices_);
        ~Rectangle();

        void draw() const;
};

#endif // !RECTANGLE_HPP
