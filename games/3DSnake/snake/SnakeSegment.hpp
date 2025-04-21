#ifndef SNAKE_SEGMENT_HPP
#define SNAKE_SEGMENT_HPP

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

struct SnakeSegment {
    glm::vec3 position;
    glm::vec3 colour;

    SnakeSegment(glm::vec3 position, glm::vec3 colour = glm::vec3(0.0f, 1.0f, 0.0f)) : position(position), colour(colour) {

    }

    inline glm::vec3& operator+(const glm::vec3& position) {
        this->position += position;
        return this->position;
    }
};

#endif // !SNAKE_SEGMENT_HPP
