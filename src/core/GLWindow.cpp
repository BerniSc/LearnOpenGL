#include "core/GLWindow.hpp"
#include "core/GLException.hpp"

GLWindow::GLWindow(int width, int height, const std::string& title, bool hasDepthTest)
    : width(width), height(height), title(title), hasDepthTest(hasDepthTest) {

    // Set our clearmask based on our "featureset"
    clearMask = 0;
    clearMask |= GL_COLOR_BUFFER_BIT;
    if(hasDepthTest)
        clearMask |= GL_DEPTH_BUFFER_BIT;
    init();

    // Prevent weird overlap of rotation cube-faces
    if(hasDepthTest)
        glEnable(GL_DEPTH_TEST);
}

GLWindow::~GLWindow() {
    glfwTerminate();
}

void GLWindow::init() {
    if(!glfwInit())
        throw GLException("Failed to initialize GLFW");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        throw GLException("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw GLException("Failed to initialize GLAD");

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

}

bool GLWindow::shouldKeepAlive() const {
    return !glfwWindowShouldClose(window);
}

void GLWindow::clear() const {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(clearMask);
}

void GLWindow::display() const {
    glfwSwapBuffers(window);
}

void GLWindow::pollEvents() const {
    glfwPollEvents();
}

GLFWwindow* GLWindow::get() const {
    return window;
}

void GLWindow::setCallbackContext(CallbackContext& context) const {
    glfwSetWindowUserPointer(window, &context);
}

CallbackContext* GLWindow::getCallbackContext() const {
    return static_cast<CallbackContext*>(glfwGetWindowUserPointer(window));
}
