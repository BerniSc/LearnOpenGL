#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include <glad/glad.h>

class VertexArray {
private:
    GLuint id;

public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void enableAttribute(GLuint index) const;
    void setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) const;

    GLuint getID() const;
};

#endif // VERTEX_ARRAY_HPP
