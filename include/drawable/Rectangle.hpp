#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "drawable/Drawable.hpp"
#include "core/ShaderProgram.hpp"
#include "core/VertexArray.hpp"
#include "core/VertexBuffer.hpp"

class Rectangle : public Drawable {
private:
    VertexArray vao;
    VertexBuffer vbo;
    const ShaderProgram& shader;

    static constexpr float defaultVertices[12] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

public:
    Rectangle(const ShaderProgram& shader, const float* vertices = defaultVertices);
    void draw() const override;
};

#endif // RECTANGLE_HPP
