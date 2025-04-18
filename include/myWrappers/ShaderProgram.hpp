#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <GL/gl.h>
#include <string>

class ShaderProgram {
    private:
        GLuint shaderProgramID = 0;
        // Use a uniform to vary our color based on the Time -> Seed our time and then interpolate our "Hue", send value to location gotten in init
        const bool varyColourTimed = false;
        int vertexColorLocation;

        // We have gotten our shaders from a File
        const bool filed = false;

        // If we dont want to load our Shader via a RAW-STRING we can load it from
        // a file to keep everything a bit more organized
        std::string loadShaderFromFile(const char* path);
        GLuint compileShader(GLenum type, const char* source);
        void compile(const char* vertexSource, const char* fragmentSource);

    public:
        ShaderProgram(const char* vertexSource, const char* fragmentSource, const bool& useUniform = false);
        ShaderProgram(bool filed, const std::string& vertexFilePath, const std::string& fragmentFilePath, bool useUniform = false);

        ~ShaderProgram();

        void use() const;

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setFloat4(const std::string& name, float valueX, float valueY, float valueZ, float valueW) const;

        GLuint id() const;
};
#endif // !SHADER_PROGRAM_HPP
