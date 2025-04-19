#include "drawable/TexturedRectangle.hpp"
#include <glad/glad.h>

// TODO FIXME Please change the string-c'tors to use a function or local factory etc. This just is not nice

// Initializer-List will implicitly create our texture-object from the path 
// FIXME DEPRECATED
TexturedRectangle::TexturedRectangle(const ShaderProgram& shader, const char* texturePath, const float* vertices)
    : shader(shader) {

    // TODO FIXME Add a way to use regular initiliazer list here? I hate just creating a local var here
    Texture tmp(texturePath);
    textures.emplace_back(&tmp);

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
    : shader(shader) {

    Texture tmp(texturePath);
    textures.emplace_back(&tmp);
    /*textures.emplace_back(texturePath);*/

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, vertexCount * stride);
    configureVertexAttributes(stride, attributes);

    vbo.unbind();
    vao.unbind();
}

TexturedRectangle::TexturedRectangle(const ShaderProgram& shader, const std::vector<std::shared_ptr<Texture>>& textures, const float* vertices)
    : shader(shader) {

    this->textures = textures;

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, sizeof(float) * 12);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vbo.unbind();
    vao.unbind();
}

TexturedRectangle::TexturedRectangle(const ShaderProgram& shader, const std::vector<std::shared_ptr<Texture>>& textures, size_t vertexCount, GLsizei stride, 
            const std::vector<VertexAttribute>& attributes, const float* vertices)
    : shader(shader) {

    this->textures = textures;

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
    for(const std::shared_ptr<Texture>&texture : this->textures)
        texture->bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    vao.unbind();
}

