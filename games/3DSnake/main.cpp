#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config/CameraConfig.hpp"
#include "core/CallbackContext.hpp"
#include "core/Camera.hpp"
#include "core/GLException.hpp"
#include "core/GLWindow.hpp"
#include "core/Shader.hpp"
#include "core/ShaderProgram.hpp"
#include "drawable/Arrow.hpp"
#include "drawable/TexturedCube.hpp"
#include "drawable/TexturedRectangle.hpp"
#include "games/3DSnake/snake/SnakeDirection.hpp"
#include "games/3DSnake/snake/SnakeHead.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

void processInput(GLWindow& window);

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,5); 

    glm::vec3 DARK_GREEN = { 0.137255, 0.55683f, 0.137255f };
    glm::vec3 LIGHT_GREEN = { 0.419608f, 0.55683f, 0.137255f };

    try {
        GLWindow window(800, 800, "3D-Snake");
        // Generate the Camera and point it at our Scene
        Camera camera(glm::vec3(5.5f, 5.5f, 10.5f));
        camera.pointCameraAt({0.0f, 0.0f, 0.0f});

        // Create a Shader to draw a simple textured and coloured shape (3d and 2d), accepts colour and transformations via uniform
        Shader snakeV(GL_VERTEX_SHADER, "games/3DSnake/shaders/snakeSegment.vs.glsl", true);
        Shader snakeF(GL_FRAGMENT_SHADER, "games/3DSnake/shaders/snakeSegment.fs.glsl", true);
        ShaderProgram snake(snakeV, snakeF);

        Shader snakeFloorV(GL_VERTEX_SHADER, "games/3DSnake/shaders/snakeFloor.vs.glsl", true);
        Shader snakeFloorF(GL_FRAGMENT_SHADER, "games/3DSnake/shaders/snakeFloor.fs.glsl", true);
        ShaderProgram snakeFloor(snakeFloorV, snakeFloorF);

        Shader arrowV(GL_VERTEX_SHADER, "games/3DSnake/shaders/arrow.vs.glsl", true);
        Shader arrowF(GL_FRAGMENT_SHADER, "games/3DSnake/shaders/arrow.fs.glsl", true);
        ShaderProgram arrow(arrowV, arrowF);

        // Pack all the textures we later might want to mix together in here
        std::vector<std::shared_ptr<Texture>> texturesSnake;
        texturesSnake.emplace_back(std::make_shared<Texture>("games/3DSnake/assets/scales.jpg", GL_TEXTURE0));

        // activate the texture, for now just the 1
        snake.setInt("texture1", 0);

        // As we default to our fallback-vertices, these contain just coordinates and Tex-Coordinates bundled in 5er packs
        std::vector<VertexAttribute> snakeAttributes = {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)},
        };
        // Create textured cube with the attributes, the expected stride for our fallbacks and its general structure
        TexturedCube snakeSegment(snake, texturesSnake, 36, 5 * sizeof(float), snakeAttributes);

        // Set our colour
        snake.setFloat3("colour", 1.0f, 0.0f, 1.0f);
        snakeFloor.setFloat3("colour", 0.419608f, 0.55683f, 0.137255f);
        snakeFloor.setFloat3("colour", 0.137255, 0.55683f, 0.137255f);

        // Currently use no textures for floor
        std::vector<std::shared_ptr<Texture>> texturesFloor;

        TexturedRectangle snakeFloorPane(snakeFloor, texturesFloor, 4, 5 * sizeof(float), snakeAttributes);

        Arrow directionalIndicator(arrow);

        // Create our Projection matrixes
        glm::mat4 model = glm::mat4(1.0f); 
        glm::mat4 view = glm::mat4(1.0f);  
        glm::mat4 projection = glm::mat4(1.0f);
        snake.setMat4("model", model);
        snake.setMat4("view", view);
        snake.setMat4("projection", projection);

        SnakeHead head;
        SnakeDirection dir = static_cast<SnakeDirection>(dist6(rng));
        head.moveSnake(dir);
        head.moveSnake(dir);
        head.moveSnake(dir);
        head.setNextColour({1.0, 0.0, 1.0});
        head.clearCreation();

        float deltaTime = 0.0f;	// Time between current frame and last frame
        float deltaTimeGameloop = 0.0f;
        float lastFrame = 0.0f; // Time of last frame

        SnakeDirection currentDirection = SNAKE_FORWARD;
        SnakeDirection lockedDirection = SNAKE_BACKWARD;

        // Create the context that we pass around in our main application! Contains everything important for gamelogic
        CallbackContext context;
        context.set<Camera>("camera", &camera);
        context.set<float>("deltaTime", &deltaTime);
        context.set<SnakeDirection>("currentDirection", &currentDirection);
        context.set<SnakeDirection>("lockedDirection", &lockedDirection);
        context.set<SnakeHead>("head", &head);
        window.setCallbackContext(context);

        while(window.shouldKeepAlive()) {
            window.clear();
            processInput(window);
            directionalIndicator.setStart({0.0f, 0.0f, 0.0f});
            directionalIndicator.setDirection(head.getVecFromEnum(currentDirection));
            directionalIndicator.setLength(1.0f);

            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            deltaTimeGameloop += deltaTime;
            lastFrame = currentFrame;  

            if(deltaTimeGameloop > 2.0f) {
                deltaTimeGameloop = 0.0f;
                head.moveSnake(currentDirection);
                directionalIndicator.setStart(head.getHeadPosition());
                camera.setCameraPosition(head.getHeadPosition() + glm::vec3(5.5f, 5.5f, 10.5f));
                camera.pointCameraAt(head.getHeadPosition());
                /*camera.pointCameraAt(head.getHeadPosition());*/
            }

            view = camera.getViewMatrix();
            snake.setMat4("view", view);
            projection = camera.getProjectionMatrix(800, 800);
            snake.setMat4("projection", projection);

            // Generate a Vector of all the needed Positions of our Snake-Segments, apply the idnividual transformation for every Element
            std::vector<glm::mat4> snakeSegmentTransforms;

            /*for(const auto& position : cubePositions) {*/
            for(const auto& segment : head.getSegments()) {
                // Translate each cube based on the first defined global "model" and then have each of them rotate in their own local space 
                // as not to move the axis of view
                /*std::cout << "Got in here with " << head.getSegments().front().position.x << head.getSegments().front().position.y << head.getSegments().front().position.z << std::endl;*/
                glm::mat4 locModel = glm::translate(model, segment.position);            // Apply translation
                locModel = glm::scale(locModel, glm::vec3(0.9, 0.9, 0.9));
                snakeSegmentTransforms.push_back(locModel);
            }

            snakeSegment.draw(snakeSegmentTransforms, "model", [&](int segmentNr) {
                //snake.setFloat3("colour", 0.0, 1.0, 0.0);
                glm::vec3 colour = head.getSegments().at(segmentNr).colour;
                snake.setFloat3("colour", colour.x, colour.y, colour.z);
            });
            
            std::vector<glm::mat4> snakeFloorTransforms;
            snakeFloor.setMat4("view", view);
            snakeFloor.setMat4("projection", projection);

            arrow.setMat4("view", view);
            arrow.setMat4("projection", projection);
            arrow.setMat4("model", model);

            const size_t WIDTH = 11;
            const size_t HEIGHT = 11;
            for(size_t i = 0; i < HEIGHT; i++) {
                for(size_t j = 0; j < WIDTH; j++) {
                    glm::mat4 locModel = glm::translate(model, glm::vec3(static_cast<float>(j), 0.0f, static_cast<float>(i)));
                    locModel = glm::rotate(locModel, static_cast<float>(glm::radians(90.0)), glm::vec3(1.0f, 0.0f, 0.0f));
                    snakeFloorTransforms.push_back(locModel);
                }
            }
            snakeFloorPane.draw(snakeFloorTransforms, "model", [&](int segmentNr) {
                //snake.setFloat3("colour", 0.0, 1.0, 0.0);
                glm::vec3 colour = DARK_GREEN;
                if(segmentNr % 2)
                    colour = LIGHT_GREEN;
                snakeFloor.setFloat3("colour", colour.x, colour.y, colour.z);
            });

            directionalIndicator.draw();

            window.display();
            window.pollEvents();
        }

    } catch(const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void processInput(GLWindow& window) {
    CallbackContext* context = window.getCallbackContext();
    Camera& cam = *context->get<Camera>("camera");
    float& deltaTime = *context->get<float>("deltaTime");

    // Quit
    if(glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window.get(), true);

    // Movement
    if(glfwGetKey(window.get(), GLFW_KEY_K) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window.get(), GLFW_KEY_J) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window.get(), GLFW_KEY_H) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window.get(), GLFW_KEY_L) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime);

    SnakeHead& head = *context->get<SnakeHead>("head");
    SnakeDirection& currentDirection = *context->get<SnakeDirection>("currentDirection");
    SnakeDirection& lockedDirection = *context->get<SnakeDirection>("lockedDirection");

    if(glfwGetKey(window.get(), GLFW_KEY_H) == GLFW_PRESS) {
        SnakeDirection next = head.getRelativeDirection('h', currentDirection);
        if (next != lockedDirection) currentDirection = next;
    }
    if (glfwGetKey(window.get(), GLFW_KEY_L) == GLFW_PRESS) {
        SnakeDirection next = head.getRelativeDirection('l', currentDirection);
        if (next != lockedDirection) currentDirection = next;
    }
    if (glfwGetKey(window.get(), GLFW_KEY_J) == GLFW_PRESS) {
        SnakeDirection next = head.getRelativeDirection('j', currentDirection);
        if (next != lockedDirection) currentDirection = next;
    }
    if (glfwGetKey(window.get(), GLFW_KEY_K) == GLFW_PRESS) {
        SnakeDirection next = head.getRelativeDirection('k', currentDirection);
        if (next != lockedDirection) currentDirection = next;
    }

    /*cam.pointCameraAt({0.0f, 0.0f, 0.0f});*/

    /*
    // Mix-Value
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mixValue += 0.001f;
        if(mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mixValue -= 0.001f;
        if(mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    */
}
