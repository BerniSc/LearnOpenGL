#include "core/VertexBuffer.hpp"

VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &id);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &id);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::setData(const void* data, GLsizeiptr size, GLenum usage) const {
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

GLuint VertexBuffer::getID() const {
    return id;
}
