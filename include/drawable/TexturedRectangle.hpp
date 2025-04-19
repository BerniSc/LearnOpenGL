#ifndef TEXTURED_RECTANGLE_HPP
#define TEXTURED_RECTANGLE_HPP

#include "drawable/Drawable.hpp"
#include "core/ShaderProgram.hpp"
#include "core/VertexArray.hpp"
#include "core/VertexBuffer.hpp"
#include "core/Texture.hpp"
#include <memory>

class TexturedRectangle : public Drawable {
private:
    VertexArray vao;
    VertexBuffer vbo;
    const ShaderProgram& shader;
    std::vector<std::shared_ptr<Texture>> textures;

    static constexpr float defaultVertices[20] = {
        // | Positions    | Tex-Coordinates
        0.3f,  0.3f, 0.0f, 1.0f, 1.0f,  // top right
        0.3f, -0.3f, 0.0f, 1.0f, 0.0f,  // bottom right
       -0.3f, -0.3f, 0.0f, 0.0f, 0.0f,  // bottom left
       -0.3f,  0.3f, 0.0f, 0.0f, 1.0f,  // top left
    };

public:
    TexturedRectangle(const ShaderProgram& shader, const char* texturePath, const float* vertices = defaultVertices);
    TexturedRectangle(const ShaderProgram& shader, const char* texturePath, size_t vertexCount, GLsizei stride, 
            const std::vector<VertexAttribute>& attributes, const float* vertices = defaultVertices);

    TexturedRectangle(const ShaderProgram& shader, const std::vector<std::shared_ptr<Texture>>& textures, const float* vertices = defaultVertices);
    TexturedRectangle(const ShaderProgram& shader, const std::vector<std::shared_ptr<Texture>>& textures, size_t vertexCount, GLsizei stride, 
            const std::vector<VertexAttribute>& attributes, const float* vertices = defaultVertices);

    void draw() const override;
};

#endif // TEXTURED_RECTANGLE_HPP
