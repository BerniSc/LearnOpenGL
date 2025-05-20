#ifndef SNAKE_HEAD_HPP
#define SNAKE_HEAD_HPP

#include "games/3DSnake/snake/SnakeDirection.hpp"
#include "games/3DSnake/snake/SnakeSegment.hpp"

#include "glm/fwd.hpp"

#include <cstddef>
#include <deque>
#include <optional>
#include <vector>

class SnakeHead {
    private:
        std::deque<SnakeSegment> snakeSegments;
        bool ateFood = true;

        glm::vec3 currentColour = glm::vec3(1.0, 0.0, 0.0);         // Colour being propagated normaly
        std::optional<glm::vec3> nextColourOverride = std::nullopt; // Optional colour override for tail

        inline void eat() {
            currentColour = snakeSegments.front().colour;
        }

        void propagateColour() {
            for(size_t i = 0; i < (snakeSegments.size() - 1); i++)
                snakeSegments.at(i).colour = snakeSegments.at(i + 1).colour;
        }

    public:
        SnakeHead() {
            snakeSegments.emplace_front(glm::vec3(0.0f, 0.0f, 0.0f), currentColour);
        }

        void inline moveSnake(const SnakeDirection& direction) {
            propagateColour();

            glm::vec3 newHead = snakeSegments.front().position + getVecFromEnum(direction);
            snakeSegments.emplace_front(newHead, currentColour);

            // If he wave not eaten we kill the last segment
            if(!ateFood) {
                snakeSegments.pop_back(); // Remove old tail
            } else {
                // Allow for overwriting the new TailColour
                if(nextColourOverride.has_value()) {
                    snakeSegments.back().colour = nextColourOverride.value();
                    nextColourOverride.reset();
                } 
            }
        }

        inline glm::vec3 getVecFromEnum(const SnakeDirection& direction) const {
            glm::vec3 movementVector = { 0, 0, 0 };
            switch(direction) {
                case SNAKE_FORWARD:
                    movementVector = { 0, 0, 1 };
                    break;
                case SNAKE_BACKWARD:
                    movementVector = { 0, 0, -1 };
                    break;
                case SNAKE_LEFT:
                    movementVector = { -1, 0, 0 };
                    break;
                case SNAKE_RIGHT:
                    movementVector = { 1, 0, 0 };
                    break;
                case SNAKE_UP:
                    movementVector = { 0, 1, 0 };
                    break;
                case SNAKE_DOWN:
                    movementVector = { 0, -1, 0 };
                    break;
            }
            return movementVector;
        }

        static inline SnakeDirection getEnumFromVec(const glm::vec3& dir) {
            glm::vec3 normalized = glm::normalize(dir);

            std::vector<std::pair<SnakeDirection, glm::vec3>> directions = {
                {SNAKE_FORWARD,  { 0,  0,  1}},
                {SNAKE_BACKWARD, { 0,  0, -1}},
                {SNAKE_LEFT,     {-1,  0,  0}},
                {SNAKE_RIGHT,    { 1,  0,  0}},
                {SNAKE_UP,       { 0,  1,  0}},
                {SNAKE_DOWN,     { 0, -1,  0}}
            };

            float maxDot = -1.0f;
            SnakeDirection bestMatch = SNAKE_FORWARD;

            for (const auto& [dirEnum, vec] : directions) {
                float dot = glm::dot(normalized, glm::normalize(vec));
                if (dot > maxDot) {
                    maxDot = dot;
                    bestMatch = dirEnum;
                }
            }

            return bestMatch;
        }

        inline SnakeDirection getRelativeDirection(char key, SnakeDirection current) {
            glm::vec3 currentVec = getVecFromEnum(current);
            glm::vec3 right = glm::normalize(glm::cross(currentVec, glm::vec3(0, 1, 0))); // world up
            glm::vec3 left = -right;
            glm::vec3 up(0, 1, 0);
            glm::vec3 down = -up;

            if(key == 'h')
                return getEnumFromVec(left);
            if(key == 'l')
                return getEnumFromVec(right);
            if(key == 'j')
                return SNAKE_DOWN;
            if(key == 'k')
                return SNAKE_UP;

            return current;
        }

        inline const std::deque<SnakeSegment>& getSegments() const {
            return snakeSegments;
        }

        inline void setNextColour(const glm::vec3& newColour) {
            /*currentColour = newColour;*/
            nextColourOverride = newColour;
        }

        inline void clearCreation() {
            this->ateFood = false;
        }

        inline const glm::vec3& getHeadPosition() const {
            return snakeSegments.front().position;
        }
};

#endif // !SNAKE_HEAD_HPP
