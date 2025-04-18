#include "myWrappers/Rectangle.hpp"

void Rectangle::initialize() {
    // Generate VBA as template-object and then VBO to store the Data
    glGenVertexArrays(1, &vertexBufferArrayID);
    glGenBuffers(1, &vertexBufferObjectID);
    // Also create EBO to "merge" our two triangles
    glGenBuffers(1, &elementBufferObjectID);

    glBindVertexArray(vertexBufferArrayID);

    // For now draw static Data -> TODO change later to allow for dynamic or stream as well
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Incices as ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices), indices, GL_STATIC_DRAW);

    // Shader starts at address 0, VBO is tightly packed
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Clean up, unbind buffer Object first, then Array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Rectangle::Rectangle(const ShaderProgram& shader, const float* customVertices) : 
    shader(shader), vertices(customVertices) {
    initialize();
}

Rectangle::~Rectangle() {
    glDeleteVertexArrays(1, &vertexBufferArrayID);
    glDeleteBuffers(1, &vertexBufferObjectID);
}

void Rectangle::draw() const {
    shader.use();
    glBindVertexArray(vertexBufferArrayID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

