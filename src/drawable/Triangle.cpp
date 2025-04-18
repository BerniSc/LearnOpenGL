#include "drawable/Triangle.hpp"
#include <glad/glad.h>

Triangle::Triangle(const ShaderProgram& shader, const float* vertices)
    : shader(shader) {

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, sizeof(float) * 9);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vbo.unbind();
    vao.unbind();
}

void Triangle::draw() const {
    shader.use();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

Triangle::~Triangle() {

}
