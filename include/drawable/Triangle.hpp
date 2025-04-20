#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "core/ShaderProgram.hpp"
#include "core/VertexArray.hpp"
#include "core/VertexBuffer.hpp"
#include "drawable/Drawable.hpp"

class Triangle : public Drawable {
private:
    VertexArray vao;
    VertexBuffer vbo;
    const ShaderProgram& shader;

    static constexpr float defaultVertices[12] = {
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

public:
    // Defaults -> Simple vertices with just position
    Triangle(const ShaderProgram& shader, const float* vertices = defaultVertices, bool wireframeMode = false);

    // Also allow for creating with more Options
    Triangle(const ShaderProgram& shader,
             const float* vertices,
             size_t vertexCount,
             GLsizei stride,
             const std::vector<VertexAttribute>& attributes,
             bool wireframeMode = false);

    ~Triangle();

    void draw() const override;
    void draw(const std::vector<glm::mat4>& transforms, const std::string& uniformName) const override;
};

#endif // TRIANGLE_HPP
