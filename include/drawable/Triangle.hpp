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

public:
    Triangle(const ShaderProgram& shader, const float* vertices);
    ~Triangle();

    void draw() const override;
};

#endif // TRIANGLE_HPP
