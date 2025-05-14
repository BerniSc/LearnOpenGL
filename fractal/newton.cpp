#include <cmath>
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/GLException.hpp"
#include "core/GLWindow.hpp"
#include "core/Shader.hpp"
#include "core/ShaderProgram.hpp"
#include "core/CallbackContext.hpp"
#include "drawable/Rectangle.hpp"
#include "glm/fwd.hpp"

constexpr float SQRT3 = 1.732050807568877f;

void processInput(GLFWwindow *window, ShaderProgram& shader) {
    auto ctx = static_cast<CallbackContext*>(glfwGetWindowUserPointer(window));
    // Roots
    glm::vec2& root1 = *(ctx->get<glm::vec2>("root1"));
    glm::vec2& root2 = *(ctx->get<glm::vec2>("root2"));
    glm::vec2& root3 = *(ctx->get<glm::vec2>("root3"));
    uint8_t& selectedRoot = *(ctx->get<uint8_t>("selectedRoot"));

    // Timing
    float& deltaTime = *(ctx->get<float>("deltaTime"));
    float& lastFrame = *(ctx->get<float>("lastFrame"));
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Zoom and Movement
    glm::vec2& center = *(ctx->get<glm::vec2>("center"));
    float& zoom = *(ctx->get<float>("zoom"));

    // Constants for movement speed
    constexpr float ZOOM_SPEED = 0.5f;
    constexpr float PAN_SPEED = 0.9f;
    constexpr float ROOT_ADJUST_SPEED = 0.1f;

    // Quit
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Root selection -> Vim-Modal-Esque
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) selectedRoot = 1;
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) selectedRoot = 2;
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) selectedRoot = 3;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) selectedRoot = 0;

    // Root manipulation when a root is selected
    if(selectedRoot > 0) {
        glm::vec2* activeRoot = nullptr;
        switch(selectedRoot) {
            case 1: activeRoot = &root1; break;
            case 2: activeRoot = &root2; break;
            case 3: activeRoot = &root3; break;
        }

        if(activeRoot) {
            // Real part adjustment
            if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
                activeRoot->x -= ROOT_ADJUST_SPEED * deltaTime;
            if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
                activeRoot->x += ROOT_ADJUST_SPEED * deltaTime;

            // Imaginary part adjustment
            if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
                activeRoot->y -= ROOT_ADJUST_SPEED * deltaTime;
            if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
                activeRoot->y += ROOT_ADJUST_SPEED * deltaTime;

            if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                root1.x = -1.2757;
                root1.y = -0.345167;
                root2.x = 0.998853;
                root2.y = -0.000889109;
                root3.x = 0.98774;
                root3.y = 2.02204;
            }
        }
    } else {

        // Zoom (a/d)
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            zoom += ZOOM_SPEED * deltaTime * zoom;  // zoom out
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            zoom -= ZOOM_SPEED * deltaTime * zoom;  // zoom in

        // Movement (hjkl)
        if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
            center.y += PAN_SPEED * deltaTime * zoom;  // up
        if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            center.y -= PAN_SPEED * deltaTime * zoom;  // down
        if(glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
            center.x -= PAN_SPEED * deltaTime * zoom;  // left
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            center.x += PAN_SPEED * deltaTime * zoom;  // right
     }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        std::cout << "root1.x = " << root1.x << ";\n" << "root1.y = " << root1.y << "\n" \
                  << "root2.x = " << root2.x << ";\n" << "root2.y = " << root2.y << "\n" \
                  << "root3.x = " << root3.x << ";\n" << "root3.y = " << root3.y << std::endl;

    // Update shader uniforms
    shader.setFloat("zz", zoom);
    shader.setVec2("center", center.x, center.y);
    shader.setVec2("root1", root1.x, root1.y);
    shader.setVec2("root2", root2.x, root2.y);
    shader.setVec2("root3", root3.x, root3.y);
}

int main() {
    try {
        constexpr unsigned width = 800;
        constexpr unsigned height = 800;
        GLWindow window(width, height, "Newton Fractal");

        // Create shaders. Want to be downwards-compatible -> No Computeshader, but only Fragmentshader
        Shader vertexShader(GL_VERTEX_SHADER, "./fractal/shader/newton.vs.glsl", true);
        Shader fragmentShader(GL_FRAGMENT_SHADER, "./fractal/shader/newton.fs.glsl", true);
        ShaderProgram shader(vertexShader, fragmentShader);

        // Just need simple vertices for full screen quad
        static constexpr float vertices[] = {
             1.0f,  1.0f, 0.0f,  // top right
             1.0f, -1.0f, 0.0f,  // bottom right
            -1.0f, -1.0f, 0.0f,  // bottom left
            -1.0f,  1.0f, 0.0f   // top left
        };

        // Setting up the CallbackContext
        CallbackContext ctx;
        // Roots and Selection
        glm::vec2 root1(1.0f, 0.0f);                    // 1 + 0i
        glm::vec2 root2(-0.5f, SQRT3 * 0.5f);           // -1/2 + (√3/2)i
        glm::vec2 root3(-0.5f, -SQRT3 * 0.5f);          // -1/2 - (√3/2)i
        uint8_t selectedRoot = 0;                       // 0 = none, 1-3 = root1-3
        // Timing
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;
        // Movement and Zoom
        glm::vec2 center(0.0f);
        float zz = 5.0f;

        ctx.set<glm::vec2>("root1", &root1);
        ctx.set<glm::vec2>("root2", &root2);
        ctx.set<glm::vec2>("root3", &root3);
        ctx.set<uint8_t>("selectedRoot", &selectedRoot);
        
        ctx.set<float>("deltaTime", &deltaTime);
        ctx.set<float>("lastFrame", &lastFrame);

        ctx.set<glm::vec2>("center", &center);
        ctx.set<float>("zoom", &zz);

        window.setCallbackContext(ctx);

        Rectangle rectangle(shader, vertices);

        // Initial setup of uniforms
        shader.setFloat("width", width);
        shader.setFloat("height", height);
        shader.setFloat("zz", zz);
        shader.setVec2("center", center.x, center.y);
        float t = (0.5 * std::sin(glfwGetTime())) + 0.5f;
        shader.setFloat("breath", t);

        shader.setFloat("sqrt3", 1.732050807568877);

        // Init colour, even if we dont want to use it, with 1 so we have them after shadermult^^
        shader.setFloat("breathR", 1.0f);
        shader.setFloat("breathG", 1.0f);
        shader.setFloat("breathB", 1.0f);

        while(window.shouldKeepAlive()) {
            processInput(window.get(), shader);
            
            // Update bearthing every iteration
            float tr = (0.5 * std::sin(glfwGetTime() / 2.0f)) + 0.5f;
            float tg = (0.5 * std::sin(glfwGetTime() * 0.25f)) + 0.5f;
            float tb = (0.5 * std::sin(glfwGetTime() * 0.6f)) + 0.5f;
            shader.setFloat("breathR", tr);
            shader.setFloat("breathG", tg);
            shader.setFloat("breathB", tb);

            window.clear();
            rectangle.draw();
            window.display();
            window.pollEvents();
        }
    } catch(const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

