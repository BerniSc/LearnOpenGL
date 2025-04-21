#ifndef SNAKE_HEAD_HPP
#define SNAKE_HEAD_HPP

#include "games/3DSnake/snake/SnakeDirection.hpp"
#include "games/3DSnake/snake/SnakeSegment.hpp"

#include "glm/fwd.hpp"

#include <deque>

class SnakeHead {
    private:
        std::deque<SnakeSegment> snakeSegments;
        bool ateFood = false;

    public:
        void inline moveSnake(const SnakeDirection& direction) {
            glm::vec3 movementVector;
            switch(direction) {
                case FORWARD:
                    movementVector = { 0, 0, 1 };
                    break;
                case BACKWARD:
                    movementVector = { 0, 0, -1 };
                    break;
                case LEFT:
                    movementVector = { -1, 0, 0 };
                    break;
                case RIGHT:
                    movementVector = { 1, 0, 0 };
                    break;
                case UP:
                    movementVector = { 0, 1, 0 };
                    break;
                case DOWN:
                    movementVector = { 0, -1, 0 };
                    break;
            }

            glm::vec3 newHead = snakeSegments.front() + movementVector;
            snakeSegments.push_front(newHead);

            // If he wave not eaten we kill the last segment
            if(!ateFood)
                snakeSegments.pop_back();
        }

};

#endif // !SNAKE_HEAD_HPP
