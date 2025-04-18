#ifndef GL_WINDOW_HPP
#define GL_WINDOW_HPP

#include "myWrappers/GLException.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <GL/gl.h>
#include <string>

void framebuffer_size_callback(GLFWwindow*, int width, int height);

class GLWindow {
    private:
        uint16_t width, height;
        std::string windowTitle;
        GLFWwindow* window = nullptr;

        void init();

    public:
        GLWindow(int width, int height, const std::string& title);

        ~GLWindow();

        bool shouldKeepAlive() const;

        void pollEvents() const;

        // "Fill" whole creen with the clear-colour
        void clear() const;

        // Swap the buffer with the now filled one
        void display() const;

        GLFWwindow* getHandle() const;
};

#endif // !GL_WINDOW_HPP
