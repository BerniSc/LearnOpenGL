#include "core/ShaderProgram.hpp"
#include <stdexcept>
#include <glad/glad.h>

ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) {
    id = glCreateProgram();
    glAttachShader(id, vertexShader.getID());
    glAttachShader(id, fragmentShader.getID());
    glLinkProgram(id);

    GLint success;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        throw std::runtime_error("Shader program linking failed:\n" + std::string(infoLog));
    }
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(id);
}

void ShaderProgram::use() const {
    glUseProgram(id);
}

GLint ShaderProgram::getUniformLocation(const std::string& name) {
    if (uniformLocations.find(name) == uniformLocations.end()) {
        uniformLocations[name] = glGetUniformLocation(id, name.c_str());
    }
    return uniformLocations[name];
}

void ShaderProgram::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setFloat4(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

GLuint ShaderProgram::getID() const {
    return id;
}
