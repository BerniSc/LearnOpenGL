#include "drawable/TexturedCube.hpp"

// For details and TODO see rect Code
// What i will add here is that it would be kinda neat to have our texture logic bundled, so MAYBE do this sometime

TexturedCube::TexturedCube(const ShaderProgram& shader, const std::vector<std::shared_ptr<Texture>>& textures, size_t vertexCount, GLsizei stride,
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

void TexturedCube::draw() const {
    shader.use();
    for(const std::shared_ptr<Texture>&texture : this->textures)
        texture->bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.unbind();
}

void TexturedCube::draw(const std::vector<glm::mat4>& transforms, const std::string& uniformName, const std::function<void(int)>& perInstanceUniformSetter) const {
    uint8_t counter = 0;
    for(const glm::mat4& mat : transforms) {
        this->shader.setMat4(uniformName, mat);

        // Set extra uniforms like color, id, etc.
        if(perInstanceUniformSetter)
            perInstanceUniformSetter(counter++);
        draw();
    }
}
