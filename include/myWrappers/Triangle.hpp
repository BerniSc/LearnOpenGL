#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "myWrappers/ShaderProgram.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <GL/gl.h>

class Triangle {
    private:
        GLuint vertexBufferArrayID = 0;
        GLuint vertexBufferObjectID = 0;
        const ShaderProgram& shader;

        bool colouredInIndex = false;

        const float* vertices;

        // Fallback vertices
        static constexpr float vertices_[9] = {
            -0.5f, -0.5f, 0.0f, // left
             0.5f, -0.5f, 0.0f, // right
             0.0f,  0.5f, 0.0f  // top
        };

        static constexpr float vertices_coloured[18] = {
        //  | positions        | colours
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // left
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // right
             0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
        };

        void initialize();

    public:
        Triangle(const ShaderProgram& shader, const float* customVertices = vertices_, bool colouredInIndex = false);
        ~Triangle();

        void draw() const;
};

#endif // !TRIANGLE_HPP
