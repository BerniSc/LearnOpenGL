#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

#include <glad/glad.h>
#include <vector>

class IndexBuffer {
private:
    GLuint bufferID = 0;
    GLsizei count = 0;

public:
    IndexBuffer();
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    void setData(const std::vector<unsigned int>& indices);
    GLsizei getCount() const;
};

#endif // INDEX_BUFFER_HPP
