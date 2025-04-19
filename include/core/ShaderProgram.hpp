#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "core/Shader.hpp"
#include <string>
#include <unordered_map>

class ShaderProgram {
private:
    GLuint id;
    // Cache our IDs on CPU Side to avoid expensive querries to the GPU
    std::unordered_map<std::string, GLint> uniformLocations;

    GLint getUniformLocation(const std::string& name);

public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void use() const;

    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string& name, float value);
    void setFloat4(const std::string& name, float x, float y, float z, float w);

    GLuint getID() const;
};

#endif // SHADER_PROGRAM_HPP
