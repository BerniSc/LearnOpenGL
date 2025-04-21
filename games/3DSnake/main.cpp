#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

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

int main() {
    try {
        GLWindow window(800, 800, "Refactored OpenGL - Going 3D");
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
