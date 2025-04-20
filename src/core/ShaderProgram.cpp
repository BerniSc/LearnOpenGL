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
    if(!success) {
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

GLint ShaderProgram::getUniformLocation(const std::string& name) const {
    // Alternative way to get the location is via slow Querry to GPU
    // glGetUniformLocation(ID, name.c_str())
    if(uniformLocations.find(name) == uniformLocations.end())
        uniformLocations[name] = glGetUniformLocation(id, name.c_str());
    return uniformLocations[name];
}

void ShaderProgram::setBool(const std::string &name, bool value) const {         
    this->use();
    glUniform1i(getUniformLocation(name), (int)value); 
}

void ShaderProgram::setInt(const std::string &name, int value) const { 
    this->use();
    glUniform1i(getUniformLocation(name), value); 
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    this->use();
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setFloat4(const std::string& name, float x, float y, float z, float w) const {
    this->use();
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    this->use();
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

GLuint ShaderProgram::getID() const {
    return id;
}
