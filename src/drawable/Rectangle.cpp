#include "drawable/Rectangle.hpp"
#include <glad/glad.h>

Rectangle::Rectangle(const ShaderProgram& shader, const float* vertices, bool wireframeMode)
    : shader(shader) {
    this->wireframeMode = wireframeMode;

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

    if(wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Can use the EBO Approach to get along with our 4 vertices by suppling 6 indices that then get drawn
    // Good approach and more efficient, but the DrawArrays works as well and is much easier to setup
    // TODO Maybe implement a efficient approach using the EBO here, but thats no Piority
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    vao.unbind();
}

void Rectangle::draw(const std::vector<glm::mat4>& transforms, const std::string& uniformName) const {
    for(const glm::mat4& mat : transforms) {
        this->shader.setMat4(uniformName, mat);
        draw();
    }
}
