#ifndef GL_WINDOW_HPP
#define GL_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class GLWindow {
private:
    GLFWwindow* window = nullptr;
    int width;
    int height;
    std::string title;

    void init();

public:
    GLWindow(int width, int height, const std::string& title);
    ~GLWindow();

    bool shouldKeepAlive() const;
    void clear() const;
    void display() const;
    void pollEvents() const;

    GLFWwindow* get() const;
};

#endif // GL_WINDOW_HPP

