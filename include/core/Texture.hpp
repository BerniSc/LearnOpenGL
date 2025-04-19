#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <cstdint>
#include <glad/glad.h>
#include <string>

class Texture {
    private:
        GLuint textureID;
        std::string filePath;

        static uint8_t instanceCounter;

        GLuint textureLocation;
    public:
        Texture(const std::string& filePath, GLuint textureLocation = GL_TEXTURE0);
        ~Texture();

        void bind() const;
        void unbind() const;

        GLuint getID() const;
        GLuint getLocation() const;
};

#endif // TEXTURE_HPP

