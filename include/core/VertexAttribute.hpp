#ifndef VERTEX_ATTRIBUTE_HPP
#define VERTEX_ATTRIBUTE_HPP

#include <glad/glad.h>
#include <cstddef>

struct VertexAttribute {
    GLuint index;               // layout location
    GLint size;                 // component count (e.g. 3 for vec3)
    GLenum type;                // usually GL_FLOAT
    GLboolean normalized;
    size_t offset;              // byte offset
};

#endif // VERTEX_ATTRIBUTE_HPP

