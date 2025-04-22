#ifndef SNAKE_HEAD_HPP
#define SNAKE_HEAD_HPP

#include "games/3DSnake/snake/SnakeDirection.hpp"
#include "games/3DSnake/snake/SnakeSegment.hpp"

#include "glm/fwd.hpp"

#include <cstddef>
#include <deque>
#include <optional>

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

            propagateColour();

            glm::vec3 newHead = snakeSegments.front().position + movementVector;
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

        const std::deque<SnakeSegment>& getSegments() const {
            return snakeSegments;
        }

        void setNextColour(const glm::vec3& newColour) {
            /*currentColour = newColour;*/
            nextColourOverride = newColour;
        }

        void clearCreation() {
            this->ateFood = false;
        }

        const glm::vec3& getHeadPosition() const {
            return snakeSegments.front().position;
        }
};

#endif // !SNAKE_HEAD_HPP
