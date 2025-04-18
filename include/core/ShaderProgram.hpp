#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include "core/Shader.hpp"
#include <string>
#include <unordered_map>

class ShaderProgram {
private:
    GLuint id;
    std::unordered_map<std::string, GLint> uniformLocations;

    GLint getUniformLocation(const std::string& name);

public:
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ~ShaderProgram();

    void use() const;

    void setFloat(const std::string& name, float value);
    void setFloat4(const std::string& name, float x, float y, float z, float w);

    GLuint getID() const;
};

#endif // SHADER_PROGRAM_HPP
