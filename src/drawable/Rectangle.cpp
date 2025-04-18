#include "drawable/Rectangle.hpp"
#include <glad/glad.h>

Rectangle::Rectangle(const ShaderProgram& shader, const float* vertices)
    : shader(shader) {

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, sizeof(float) * 12);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vbo.unbind();
    vao.unbind();
}

void Rectangle::draw() const {
    shader.use();
    vao.bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
