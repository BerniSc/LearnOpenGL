#include "core/Texture.hpp"
#include "core/GLException.hpp"
#include "stbInclude.hpp"
#include <iostream>

Texture::Texture(const std::string& filePath, GLuint textureLocation) : filePath(filePath), textureLocation(textureLocation) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    instanceCounter++;

    // TODO Make these configurable
    // set the texture wrapping/filtering options (on the currently bound texture object)
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

    // Load image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if(data) {
        GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        // Automatically generate a Mipmap for this image without having to increment second arg of function glTexImage and loop
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        throw GLException("Failed to load Texture! " + filePath);
    }
    stbi_image_free(data);
}

Texture::~Texture() {
    std::cout << "Deleted " << textureID << std::endl;
    glDeleteTextures(1, &textureID);
}

void Texture::bind() const {
    glActiveTexture(textureLocation);
    /*std::cout << this->getLocation() << "|" << textureID << std::endl;*/
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getID() const {
    return textureID;
}

GLuint Texture::getLocation() const {
    return (this->textureLocation - GL_TEXTURE0);
}

uint8_t Texture::instanceCounter = 0;
