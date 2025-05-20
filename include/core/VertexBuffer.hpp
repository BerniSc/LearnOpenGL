#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP

#include <glad/glad.h>

class VertexBuffer {
private:
    GLuint id;

public:
    VertexBuffer();
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void setData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW) const;

    GLuint getID() const;
};

#endif // VERTEX_BUFFER_HPP
