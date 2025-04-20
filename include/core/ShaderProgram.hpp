#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "core/Shader.hpp"
#include "glm/ext/matrix_float4x4.hpp"

#include <string>
#include <unordered_map>

class ShaderProgram {
private:
    GLuint id;
    // Cache our IDs on CPU Side to avoid expensive querries to the GPU
    mutable std::unordered_map<std::string, GLint> uniformLocations;

    GLint getUniformLocation(const std::string& name) const;

public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void use() const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloat4(const std::string& name, float x, float y, float z, float w) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    GLuint getID() const;
};

#endif // SHADER_PROGRAM_HPP
