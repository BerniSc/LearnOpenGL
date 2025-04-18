#include "myWrappers/ShaderProgram.hpp"
#include "myWrappers/GLException.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

std::string ShaderProgram::loadShaderFromFile(const char* path) {
    std::ifstream file;
    // Ensure or ifstream can throw exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::stringstream buffer;

    try {
        file.open(path);
        // TODO Use GL Exception here? Its not really GL tho
        if(!file.is_open())
            throw std::runtime_error(std::string("Failed to open shader file: ") + path);
        buffer << file.rdbuf();
    } catch(std::ifstream::failure& ex) {
        std::cerr << "ERROR::SHADER_FILE_READ: " << ex.what() << std::endl;
        throw GLException("Shader file could not be read!");
    }
    std::cout << buffer.str().c_str();
    return buffer.str().c_str();
}

GLuint ShaderProgram::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        throw GLException(std::string("Shader compilation failed: ") + log);
    }

    return shader;
}

void ShaderProgram::compile(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    glLinkProgram(shaderProgramID);

    GLint success;
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        char log[512];
        glGetProgramInfoLog(shaderProgramID, sizeof(log), nullptr, log);
        throw GLException(std::string("Shader linking failed: ") + log);
    }

    // Clean up trash -> Used shaders and bundled into Program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if(this->varyColourTimed) {
        vertexColorLocation = glGetUniformLocation(shaderProgramID, "uniformedColor");
        if(vertexColorLocation == -1)
            std::cerr << "Our uniform has not been found. Shit!\n";
    }
}

ShaderProgram::ShaderProgram(const char* vertexSource, const char* fragmentSource, const bool& useUniform) : varyColourTimed(useUniform) {
    compile(vertexSource, fragmentSource);
}

ShaderProgram::ShaderProgram(bool filed, const std::string& vertexFilePath, const std::string& fragmentFilePath, bool useUniform)
    : varyColourTimed(useUniform), filed(filed) {
    std::string vShaderCode = loadShaderFromFile(vertexFilePath.c_str());
    std::string fShaderCode = loadShaderFromFile(fragmentFilePath.c_str());
    compile(vShaderCode.c_str(), fShaderCode.c_str());
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shaderProgramID);
}

void ShaderProgram::use() const {
    glUseProgram(shaderProgramID);
    if(this->varyColourTimed) {
        float timeValue = glfwGetTime();
        float greenHue = ((sin(timeValue) / 2.0f) + 0.5f);
        greenHue = (sin(glfwGetTime()) / 2.0f) + 0.5f;
        glUniform4f(vertexColorLocation, 0.0f, greenHue, 0.0f, 1.0f);
    }
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
    glUseProgram(shaderProgramID);
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    glUseProgram(shaderProgramID);
    glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
    glUseProgram(shaderProgramID);
    glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void ShaderProgram::setFloat4(const std::string& name, float valueX, float valueY, float valueZ, float valueW) const {
    glUseProgram(shaderProgramID);
    glUniform4f(glGetUniformLocation(shaderProgramID, name.c_str()), valueX, valueY, valueZ, valueW);
}

GLuint ShaderProgram::id() const {
    return shaderProgramID;
}

