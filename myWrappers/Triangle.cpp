#include "myWrappers/Triangle.hpp"

void Triangle::initialize() {
    // Generate VBA as template-object and then VBO to store the Data
    glGenVertexArrays(1, &vertexBufferArrayID);
    glGenBuffers(1, &vertexBufferObjectID);

    glBindVertexArray(vertexBufferArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);

    // Shader starts at address 0, VBO is tightly packed
    if(this->colouredInIndex) {
        // For now draw static Data -> TODO change later to allow for dynamic or stream as well
        glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(vertices_coloured), vertices_coloured, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    } else {
        // For now draw static Data -> TODO change later to allow for dynamic or stream as well
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Elements are just regular 4 floats broad -> Seperate case
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);
    }

    // Clean up, unbind buffer Object first, then Array
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Triangle::Triangle(const ShaderProgram& shader, const float* customVertices, bool colouredInIndex) : 
    shader(shader), colouredInIndex(colouredInIndex), vertices(customVertices) {
    initialize();
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &vertexBufferArrayID);
    glDeleteBuffers(1, &vertexBufferObjectID);
}

void Triangle::draw() const {
    shader.use();
    glBindVertexArray(vertexBufferArrayID);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
