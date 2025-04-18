#include "core/VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
}

void VertexArray::bind() const {
    glBindVertexArray(id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::enableAttribute(GLuint index) const {
    glEnableVertexAttribArray(index);
}

void VertexArray::setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const {
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

GLuint VertexArray::getID() const {
    return id;
}
