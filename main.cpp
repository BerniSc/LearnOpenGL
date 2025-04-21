#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>

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
#include "drawable/Triangle.hpp"
#include "drawable/Rectangle.hpp"
#include "glm/fwd.hpp"

/*void processInput(GLFWwindow *window, float& mixValue);*/
/*void processInput(GLFWwindow *window, float& mixValue, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp);*/
void processInput(GLFWwindow *window, Camera& cam, float& mixValue);
void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
void processGamepadInput(Camera& camera);

// Timing -> Maybe move to cam?
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double lastX = 800 / 2.0f;
double lastY = 800 / 2.0f;
bool firstMouse = true;

int main() {
    try {
        GLWindow window(800, 800, "Refactored OpenGL - Going 3D");

        /*glfwSetCursorPosCallback(window.get(), mouseCallback);*/
        /*glfwSetScrollCallback(window.get(), scroll_callback);*/

        // tell GLFW to capture our mouse -> DOES NOT WORK in WSL
        /*glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/

        // Create our Projection matrixes
        glm::mat4 model = glm::mat4(1.0f);                                              // Model translates from local to global
        /*model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); */

        glm::mat4 view = glm::mat4(1.0f);                                               // View translates global to "camera"
        // translating the scene reverse of where we want to move our camera (camera stays "fixed", we move the scene)
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);

        glm::mat4 projection;                                                           // Projection transforms "camera" to clip
        // Want to use perspective projection instead of orthografic for "realism" :-)
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // Load our shaders from files
        Shader texture3DV(GL_VERTEX_SHADER, "./shaders/3DTexture.vs.glsl", true);
        Shader texture3DF(GL_FRAGMENT_SHADER, "./shaders/3DTexture.fs.glsl", true);
        ShaderProgram texture3D(texture3DV, texture3DF);

        // Setup the texure
        std::vector<std::shared_ptr<Texture>> textures;
        textures.emplace_back(std::make_shared<Texture>("./assets/wall.jpg", GL_TEXTURE0));
        textures.emplace_back(std::make_shared<Texture>("./assets/awesomeface.png", GL_TEXTURE1));

        static constexpr float rectangleVertices[20] = {
            // coordinates    | TexCoords
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // Top Right
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left
           -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top Left
        };

        std::vector<VertexAttribute> rectangleAttributes = {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)},
        };

        TexturedRectangle rectangle(texture3D, textures, 4, 5 * sizeof(float), rectangleAttributes, rectangleVertices);
        TexturedCube cube(texture3D, textures, 36, 5 * sizeof(float), rectangleAttributes);

        float mixValue = 0.2f;

        texture3D.setInt("texture1", 0);
        texture3D.setInt("texture2", 1);
        texture3D.setFloat("mixValue", mixValue);

        // Place mutliple Cubes using these positions and one single instance
        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f), 
            glm::vec3( 2.0f,  5.0f, -15.0f), 
            glm::vec3(-1.5f, -2.2f, -2.5f),  
            glm::vec3(-3.8f, -2.0f, -12.3f),  
            glm::vec3( 2.4f, -0.4f, -3.5f),  
            glm::vec3(-1.7f,  3.0f, -7.5f),  
            glm::vec3( 1.3f, -2.0f, -2.5f),  
            glm::vec3( 1.5f,  2.0f, -2.5f), 
            glm::vec3( 1.5f,  0.2f, -1.5f), 
            glm::vec3(-1.3f,  1.0f, -1.5f)  
        };

        // Prevent weird overlap of rotation cube-faces
        glEnable(GL_DEPTH_TEST);  

        glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        CallbackContext ctx;
        ctx.set<Camera>("camera", &camera);
        ctx.set<float>("mixValue", &mixValue);
        ctx.set<bool>("firstMouse", &firstMouse);
        window.setCallbackContext(ctx);

        // Main render loop
        while(window.shouldKeepAlive()) {

            /*processInput(window.get(), mixValue, cameraPos, cameraFront, cameraUp);*/
            processInput(window.get(), camera, mixValue);
            processGamepadInput(camera);
            texture3D.setFloat("mixValue", mixValue);
            window.clear();

            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;  

            /*view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);*/

            view = camera.getViewMatrix();
            int viewLoc = glGetUniformLocation(texture3D.getID(), "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            projection = camera.getProjectionMatrix(800, 800);
            int projectionLoc = glGetUniformLocation(texture3D.getID(), "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            // Generate a Vector of all the needed Positions of our Cube, apply 
            std::vector<glm::mat4> cubeTransforms;

            for(const auto& position : cubePositions) {
                // Translate each cube based on the first defined global "model" and then have each of them rotate in their own local space 
                // as not to move the axis of view
                glm::mat4 locModel = glm::translate(model, position);            // Apply translation

                float angle = (float)glfwGetTime() * glm::radians(50.0f);        // Time-based rotation
                locModel = glm::rotate(locModel, angle, glm::vec3(1.0f, 1.0f, 0.0f));

                cubeTransforms.push_back(locModel);
            }

            int modelLoc = glGetUniformLocation(texture3D.getID(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            /*rectangle.draw();*/
            cube.draw(cubeTransforms, "model");

            window.display();
            window.pollEvents();


            // Clear our Depth-Buffer and also the colourbuffer to allow for it dynamically changing each iteration
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    } catch(const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void processInput(GLFWwindow *window, Camera& cam, float& mixValue) {
    // Quit
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Movement
    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime);

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
}

void mouseCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    auto ctx = static_cast<CallbackContext*>(glfwGetWindowUserPointer(window));
    Camera* cam = ctx->get<Camera>("camera");
    /*bool* firstMouse = ctx->get<bool>("firstMouse");*/

    if(firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
    
    std::cout << lastX << " | " << lastY << " --- " << xOffset << " | " << yOffset << std::endl;

    lastX = xPos;
    lastY = yPos;

    cam->ProcessMouseMovement(xOffset, yOffset);

            glfwSetCursorPos(window, 400, 400);
}

void processGamepadInput(Camera& camera) {
int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
std::cout << present << std::endl;
    std::cout << "STILL HERE0" << std::endl;
    if(!glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
        return;

    std::cout << "STILL HERE1" << std::endl;
    GLFWgamepadstate state;
    if (!glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
        return;
    std::cout << "STILL HERE2" << std::endl;

    // Read right stick (for look)
    float xOffset = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
    float yOffset = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

    // Deadzone to prevent drift
    const float deadzone = 0.1f;
    if (std::abs(xOffset) < deadzone) xOffset = 0.0f;
    if (std::abs(yOffset) < deadzone) yOffset = 0.0f;

    std::cout << xOffset << " | " << yOffset << std::endl;
    // Reuse your mouse sensitivity config
    if (xOffset != 0.0f || yOffset != 0.0f) {
        // Scale like mouse input
        camera.ProcessMouseMovement(xOffset * 10.0f, -yOffset * 10.0f);
    }
}
