#include "drawable/Triangle.hpp"
#include <glad/glad.h>

Triangle::Triangle(const ShaderProgram& shader, const float* vertices, bool wireframeMode)
    : shader(shader) {
    this->wireframeMode = wireframeMode;
    vao.bind();
    vbo.bind();

    // TODO Refactor to check for size and then also use the new configurefunction from superclass
    vbo.setData(vertices, sizeof(float) * 9);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vbo.unbind();
    vao.unbind();
}

Triangle::Triangle(const ShaderProgram& shader, const float* vertices, size_t vertexCount, GLsizei stride,
                    const std::vector<VertexAttribute>& attributes, bool wireframeMode)
    : shader(shader) {
    this->wireframeMode = wireframeMode;

    vao.bind();
    vbo.bind();

    vbo.setData(vertices, vertexCount * stride);
    configureVertexAttributes(stride, attributes);

    vbo.unbind();
    vao.unbind();
}

void Triangle::draw() const {
    shader.use();
    vao.bind();

    if(wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    vao.unbind();
}

void Triangle::draw(const std::vector<glm::mat4>& transforms, const std::string& uniformName) const {
    for(const glm::mat4& mat : transforms) {
        this->shader.setMat4(uniformName, mat);
        draw();
    }
}

Triangle::~Triangle() {

}
