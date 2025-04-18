#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>

class Texture {
private:
    GLuint id;
    std::string filePath;

public:
    Texture(const std::string& filePath);
    ~Texture();

    void bind() const;
    void unbind() const;

    GLuint getID() const;
};

#endif // TEXTURE_HPP

