#include "core/IndexBuffer.hpp"

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &bufferID);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &bufferID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::setData(const std::vector<unsigned int>& indices) {
    count = static_cast<GLsizei>(indices.size());
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

GLsizei IndexBuffer::getCount() const {
    return count;
}
