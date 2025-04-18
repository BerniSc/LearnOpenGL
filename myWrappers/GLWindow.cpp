#include "myWrappers/GLWindow.hpp"

#include <iostream>

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    std::cout << "Resized?";
    glViewport(0, 0, width, height);
}

void GLWindow::init() {
    if(!glfwInit())
        throw GLException("Failed to initialize GLFW.");
    // Init the Window using version 3.3 with Core Mode
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        throw GLException("Failed to create GLFW window!");
    }

    // Init the Window
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Init the GLAD Loader to setup the correct addresses 
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!\n";
        throw GLException("Failed to initialize GLAD!");
    }

    glViewport(0, 0, width, height);
}

GLWindow::GLWindow(int width, int height, const std::string& title)
    : width(width), height(height), windowTitle(title) {
    init();
}

GLWindow::~GLWindow() {
    glfwTerminate();
}

bool GLWindow::shouldKeepAlive() const {
    return !glfwWindowShouldClose(window);
}

void GLWindow::pollEvents() const {
    glfwPollEvents();
}

// "Fill" whole creen with the clear-colour
void GLWindow::clear() const {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Swap the buffer with the now filled one
void GLWindow::display() const {
    glfwSwapBuffers(window);
}

GLFWwindow* GLWindow::getHandle() const {
    return window;
}
