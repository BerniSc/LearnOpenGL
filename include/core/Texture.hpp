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

        // TODO FIXME make parametrizable! This is just for testing and sucks
        inline void setStuff() {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        GLuint getID() const;
        GLuint getLocation() const;
};

#endif // TEXTURE_HPP

