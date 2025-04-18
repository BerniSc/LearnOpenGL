#ifndef TEXTURED_RECTANGLE_HPP
#define TEXTURED_RECTANGLE_HPP

#include "drawable/Drawable.hpp"
#include "core/ShaderProgram.hpp"
#include "core/VertexArray.hpp"
#include "core/VertexBuffer.hpp"
#include "core/Texture.hpp"

class TexturedRectangle : public Drawable {
private:
    VertexArray vao;
    VertexBuffer vbo;
    const ShaderProgram& shader;
    Texture texture;

    static constexpr float defaultVertices[12] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
    };

public:
    TexturedRectangle(const ShaderProgram& shader, const char* texturePath, const float* vertices = defaultVertices);
    void draw() const override;
};

#endif // TEXTURED_RECTANGLE_HPP
