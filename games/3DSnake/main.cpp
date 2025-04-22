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
#include "drawable/TexturedCube.hpp"
#include "drawable/TexturedRectangle.hpp"
#include "games/3DSnake/snake/SnakeDirection.hpp"
#include "games/3DSnake/snake/SnakeHead.hpp"

void processInput(GLWindow& window);

int main() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,5); 

    try {
        GLWindow window(800, 800, "3D-Snake");
        // Generate the Camera and point it at our Scene
        Camera camera(glm::vec3(5.5f, 5.5f, 10.5f));
        camera.pointCameraAt({0.0f, 0.0f, 0.0f});

        // Create a Shader to draw a simple textured and coloured shape (3d and 2d), accepts colour and transformations via uniform
        Shader snakeV(GL_VERTEX_SHADER, "games/3DSnake/shaders/snakeSegment.vs.glsl", true);
        Shader snakeF(GL_FRAGMENT_SHADER, "games/3DSnake/shaders/snakeSegment.fs.glsl", true);
        ShaderProgram snake(snakeV, snakeF);

        // Pack all the textures we later might want to mix together in here
        std::vector<std::shared_ptr<Texture>> textures;
        textures.emplace_back(std::make_shared<Texture>("games/3DSnake/assets/scales.jpg", GL_TEXTURE0));

        // activate the texture, for now just the 1
        snake.setInt("texture1", 0);

        // As we default to our fallback-vertices, these contain just coordinates and Tex-Coordinates bundled in 5er packs
        std::vector<VertexAttribute> snakeAttributes = {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)},
        };
        // Create textured cube with the attributes, the expected stride for our fallbacks and its general structure
        TexturedCube snakeSegment(snake, textures, 36, 5 * sizeof(float), snakeAttributes);

        // Set our colour
        snake.setFloat3("colour", 1.0f, 0.0f, 1.0f);

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

        /*glm::vec3 cubePositions[] = {*/
        /*    glm::vec3( 0.0f,  0.0f,  0.0f), */
        /*    glm::vec3( 2.0f,  5.0f, -15.0f), */
        /*    glm::vec3(-1.5f, -2.2f, -2.5f),  */
        /*    glm::vec3(-3.8f, -2.0f, -12.3f),  */
        /*    glm::vec3( 2.4f, -0.4f, -3.5f),  */
        /*    glm::vec3(-1.7f,  3.0f, -7.5f),  */
        /*    glm::vec3( 1.3f, -2.0f, -2.5f),  */
        /*    glm::vec3( 1.5f,  2.0f, -2.5f), */
        /*    glm::vec3( 1.5f,  0.2f, -1.5f), */
        /*    glm::vec3(-1.3f,  1.0f, -1.5f)  */
        /*};*/

        float deltaTime = 0.0f;	// Time between current frame and last frame
        float deltaTimeGameloop = 0.0f;
        float lastFrame = 0.0f; // Time of last frame

        // Create the context that we pass around in our main application! Contains everything important for gamelogic
        CallbackContext context;
        context.set<Camera>("camera", &camera);
        context.set<float>("deltaTime", &deltaTime);
        window.setCallbackContext(context);

        while(window.shouldKeepAlive()) {
            window.clear();
            processInput(window);

            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            deltaTimeGameloop += deltaTime;
            lastFrame = currentFrame;  

            if(deltaTimeGameloop > 2.0f) {
                deltaTimeGameloop = 0.0f;
                head.moveSnake(dir);
                camera.setCameraPosition(head.getHeadPosition() + glm::vec3(5.5f, 5.5f, 10.5f));
                camera.pointCameraAt(head.getHeadPosition());
                /*camera.pointCameraAt(head.getHeadPosition());*/
            }

            view = camera.getViewMatrix();
            snake.setMat4("view", view);
            projection = camera.getProjectionMatrix(800, 800);
            snake.setMat4("projection", projection);

            // Generate a Vector of all the needed Positions of our Cube, apply 
            std::vector<glm::mat4> cubeTransforms;

            /*for(const auto& position : cubePositions) {*/
            for(const auto& segment : head.getSegments()) {
                // Translate each cube based on the first defined global "model" and then have each of them rotate in their own local space 
                // as not to move the axis of view
                /*std::cout << "Got in here with " << head.getSegments().front().position.x << head.getSegments().front().position.y << head.getSegments().front().position.z << std::endl;*/
                glm::mat4 locModel = glm::translate(model, segment.position);            // Apply translation
                locModel = glm::scale(locModel, glm::vec3(0.9, 0.9, 0.9));
                cubeTransforms.push_back(locModel);
            }

            snakeSegment.draw(cubeTransforms, "model", [&](int segmentNr) {
                /*snake.setFloat3("colour", 0.0, 1.0, 0.0);*/
                glm::vec3 colour = head.getSegments().at(segmentNr).colour;
                snake.setFloat3("colour", colour.x, colour.y, colour.z);
            });

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
