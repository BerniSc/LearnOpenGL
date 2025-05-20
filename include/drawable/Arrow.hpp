#ifndef ARROW_HPP
#define ARROW_HPP

#include "drawable/Drawable.hpp"
#include "core/ShaderProgram.hpp"
#include "core/VertexArray.hpp"
#include "core/VertexBuffer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Arrow : public Drawable {
    private:
        VertexArray vao;
        VertexBuffer vbo;
        const ShaderProgram& shader;

        glm::vec3 start = {0.0f, 0.0f, 0.0f};
        glm::vec3 direction = {0.0f, 0.0f, 1.0f};
        glm::vec3 color = {1.0f, 1.0f, 0.0f}; // default yellow
        float length = 1.0f;

        void updateVertexData() const {
            glm::vec3 dirNorm = glm::normalize(direction);
            glm::vec3 end = start + dirNorm * length;

            glm::vec3 tipBase = end - dirNorm * 0.2f * length;

            // Build a small triangle cone at the tip (just one triangle for simplicity)
            glm::vec3 side1 = glm::cross(dirNorm, glm::vec3(0.0f, 1.0f, 0.0f));
            if (glm::length(side1) < 0.001f)
                side1 = glm::cross(dirNorm, glm::vec3(1.0f, 0.0f, 0.0f));

            side1 = glm::normalize(side1) * 0.05f * length;
            glm::vec3 p1 = tipBase + side1;
            glm::vec3 p2 = tipBase - side1;

            float vertices[] = {
                // Shaft (line style)
                start.x, start.y, start.z, color.r, color.g, color.b,
                tipBase.x, tipBase.y, tipBase.z, color.r, color.g, color.b,

                // Tip triangle
                end.x, end.y, end.z, color.r, color.g, color.b,
                p1.x, p1.y, p1.z, color.r, color.g, color.b,
                p2.x, p2.y, p2.z, color.r, color.g, color.b
            };

            vbo.bind();
            vbo.setData(vertices, sizeof(vertices));
            vbo.unbind();
        }

    public:
        Arrow(const ShaderProgram& shader, bool wireframeMode = false)
            : shader(shader) {
            this->wireframeMode = wireframeMode;

            vao.bind();
            vbo.bind();

            // Allocate enough space for shaft (2 points) and triangle (3 points)
            vbo.setData(nullptr, sizeof(float) * 5 * 6); // 5 vertices x (3 pos + 3 color)

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            vbo.unbind();
            vao.unbind();
        }

        void setStart(const glm::vec3& s) { start = s; }
        void setDirection(const glm::vec3& dir) { direction = dir; }
        void setLength(float len) { length = len; }
        void setColor(const glm::vec3& col) { color = col; }

        void draw() const override {
            shader.use();
            updateVertexData();

            vao.bind();
            if (wireframeMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            // Draw shaft (first 2 vertices)
            glDrawArrays(GL_LINES, 0, 2);

            // Draw triangle tip (next 3 vertices)
            glDrawArrays(GL_TRIANGLES, 2, 3);

            if (wireframeMode)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            vao.unbind();
        }

        void draw(const std::vector<glm::mat4>& transforms, const std::string& uniformName,
                  const std::function<void(int)>& perInstanceUniformSetter = nullptr) const override {
            int index = 0;
            for (const auto& mat : transforms) {
                shader.setMat4(uniformName, mat);
                if (perInstanceUniformSetter)
                    perInstanceUniformSetter(index++);
                draw();
            }
        }
};

#endif // ARROW_HPP
