#include "core/Shader.hpp"
#include "myWrappers/GLException.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

Shader::Shader(GLenum shaderType, const std::string& source, bool isFile)
    : type(shaderType) {
    std::string shaderSource = (isFile ? loadSourceFromFile(source) : source);
    compile(shaderSource);
}

Shader::~Shader() {
    glDeleteShader(id);
}

std::string Shader::loadSourceFromFile(const std::string& filepath) {
    std::cout << "Trying to load from file";
    std::cout << filepath;
    std::ifstream file(filepath.c_str());
    if(!file.is_open())
        throw std::runtime_error("Failed to open shader file: " + filepath);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Shader::compile(const std::string& source) {
    const char* src = source.c_str();
    id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        throw GLException("Shader compilation failed:\n" + std::string(infoLog));
    }
}

GLuint Shader::getID() const {
    return id;
}
