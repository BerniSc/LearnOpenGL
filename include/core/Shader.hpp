#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>

class Shader {
private:
    GLuint id;
    GLenum type;

    std::string loadSourceFromFile(const std::string& filepath);
    void compile(const std::string& source);

public:
    Shader(GLenum shaderType, const std::string& source, bool isFile = false);
    ~Shader();

    GLuint getID() const;
};

#endif // SHADER_HPP
