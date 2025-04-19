#include "drawable/TexturedRectangle.hpp"
#include <glad/glad.h>

// Initializer-List will implicitly create our texture-object from the path 
TexturedRectangle::TexturedRectangle(const ShaderProgram& shader, const char* texturePath, const float* vertices)
    : shader(shader), texture(texturePath) {

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, sizeof(float) * 12);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vbo.unbind();
    vao.unbind();
}

// Initializer-List will implicitly create our texture-object from the path 
TexturedRectangle::TexturedRectangle(const ShaderProgram& shader, const char* texturePath, size_t vertexCount, GLsizei stride,
                    const std::vector<VertexAttribute>& attributes, const float* vertices)
    : shader(shader), texture(texturePath) {

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, vertexCount * stride);
    configureVertexAttributes(stride, attributes);

    vbo.unbind();
    vao.unbind();
}

// For details and TODO see rect Code
void TexturedRectangle::draw() const {
    shader.use();
    texture.bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    vao.unbind();
}

