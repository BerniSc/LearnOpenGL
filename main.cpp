// main.cpp

#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <ostream>
#include <string>

const char* vertexShaderTriangleSrc = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
   gl_Position = vec4(aPos, 1.0);
}
)glsl";

const char* fragmentShaderOrangeSrc = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)glsl";

const char* fragmentShaderRedSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(1.0, 0.2, 0.1, 0.7);
}
)glsl";

const char* fragmentShaderGreenSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(0.1, 1.0, 0.4, 0.7);
}
)glsl";

const char* fragmentShaderBlueSource = R"glsl(
#version 330 core
out vec4 FragColor;
void main() {
   FragColor = vec4(0.1, 0.4, 1.0, 0.7);
}
)glsl";



class GLException {
    private:
        std::string reason;

    public:
        explicit GLException(const std::string& message) : reason(message) {}
        const std::string& what() const { return reason; }
};

std::ostream& operator<<(std::ostream& os, const GLException& ex) {
    return os << "GLException: " << ex.what();
}

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    std::cout << "Resized?";
    glViewport(0, 0, width, height);
}

class ShaderProgram {
    private:
        GLuint shaderProgramID = 0;

        GLuint compileShader(GLenum type, const char* source) {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success) {
                char log[512];
                glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
                throw GLException(std::string("Shader compilation failed: ") + log);
            }

            return shader;
        }

        void compile(const char* vertexSource, const char* fragmentSource) {
            GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
            GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

            shaderProgramID = glCreateProgram();
            glAttachShader(shaderProgramID, vertexShader);
            glAttachShader(shaderProgramID, fragmentShader);
            glLinkProgram(shaderProgramID);

            GLint success;
            glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
            if(!success) {
                char log[512];
                glGetProgramInfoLog(shaderProgramID, sizeof(log), nullptr, log);
                throw GLException(std::string("Shader linking failed: ") + log);
            }

            // Clean up trash -> Used shaders and bundled into Program
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

    public:
        ShaderProgram(const char* vertexSource, const char* fragmentSource) {
            compile(vertexSource, fragmentSource);
        }

        ~ShaderProgram() {
            glDeleteProgram(shaderProgramID);
        }

        void use() const {
            glUseProgram(shaderProgramID);
        }

        GLuint id() const {
            return shaderProgramID;
        }

};

class Triangle {
    private:
        GLuint vertexBufferArrayID = 0;
        GLuint vertexBufferObjectID = 0;
        const ShaderProgram& shader;

        const float* vertices;

        // Fallback vertices
        static constexpr float vertices_[9] = {
            -0.5f, -0.5f, 0.0f, // left
             0.5f, -0.5f, 0.0f, // right
             0.0f,  0.5f, 0.0f  // top
        };

        void initialize() {
            // Generate VBA as template-object and then VBO to store the Data
            glGenVertexArrays(1, &vertexBufferArrayID);
            glGenBuffers(1, &vertexBufferObjectID);

            glBindVertexArray(vertexBufferArrayID);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
            // For now draw static Data -> TODO change later to allow for dynamic or stream as well
            glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Shader starts at address 0, VBO is tightly packed
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            // Clean up, unbind buffer Object first, then Array
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

    public:
        Triangle(const ShaderProgram& shader, const float* customVertices = vertices_) : 
            shader(shader), vertices(customVertices) {
            initialize();
        }

        ~Triangle() {
            glDeleteVertexArrays(1, &vertexBufferArrayID);
            glDeleteBuffers(1, &vertexBufferObjectID);
        }

        void draw() const {
            shader.use();
            glBindVertexArray(vertexBufferArrayID);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
};

class Rectangle {
    private:
        GLuint elementBufferObjectID = 0;
        GLuint vertexBufferArrayID = 0;
        GLuint vertexBufferObjectID = 0;
        const ShaderProgram& shader;

        const float* vertices;

        // Our recangle consists of two triangles as OpenGL is designed to work with those
        // As we do not want to specify unneccesary points and send them to the GPU as this 
        // is expensive we use this approach with 4 points and then indices 
        // We 
        // Fallback vertices
        static constexpr float vertices_[12] = {
             0.5f,  0.5f, 0.0f,  // top right
             0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };

        unsigned int indices[6] = {  
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        void initialize() {
            // Generate VBA as template-object and then VBO to store the Data
            glGenVertexArrays(1, &vertexBufferArrayID);
            glGenBuffers(1, &vertexBufferObjectID);
            // Also create EBO to "merge" our two triangles
            glGenBuffers(1, &elementBufferObjectID);

            glBindVertexArray(vertexBufferArrayID);

            // For now draw static Data -> TODO change later to allow for dynamic or stream as well
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);
            glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(vertices), vertices, GL_STATIC_DRAW);

            // Incices as ELEMENT_ARRAY_BUFFER
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObjectID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(indices), indices, GL_STATIC_DRAW);

            // Shader starts at address 0, VBO is tightly packed
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            // Clean up, unbind buffer Object first, then Array
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

    public:
        Rectangle(const ShaderProgram& shader, const float* customVertices = vertices_) : 
            shader(shader), vertices(customVertices) {
            initialize();
        }

        ~Rectangle() {
            glDeleteVertexArrays(1, &vertexBufferArrayID);
            glDeleteBuffers(1, &vertexBufferObjectID);
        }

        void draw() const {
            shader.use();
            glBindVertexArray(vertexBufferArrayID);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }
};

class GLWindow {
    private:
        uint16_t width, height;
        std::string windowTitle;
        GLFWwindow* window = nullptr;

        void init() {
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

    public:
        GLWindow(int width, int height, const std::string& title)
            : width(width), height(height), windowTitle(title) {
            init();
        }

        ~GLWindow() {
            glfwTerminate();
        }

        bool shouldKeepAlive() const {
            return !glfwWindowShouldClose(window);
        }

        void pollEvents() const {
            glfwPollEvents();
        }

        // "Fill" whole creen with the clear-colour
        void clear() const {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        // Swap the buffer with the now filled one
        void display() const {
            glfwSwapBuffers(window);
        }

        GLFWwindow* getHandle() const {
            return window;
        }
};

int main() {
    try {
        GLWindow window(800, 800, "Clean Triangle");

        ShaderProgram shader(vertexShaderTriangleSrc, fragmentShaderOrangeSrc);
        ShaderProgram shaderRedTriangle(vertexShaderTriangleSrc, fragmentShaderRedSource);
        ShaderProgram shaderGreen(vertexShaderTriangleSrc, fragmentShaderGreenSource);
        ShaderProgram shaderBlue(vertexShaderTriangleSrc, fragmentShaderBlueSource);

        Triangle triangle(shader);

        // x-y-z regular coordinate-system
        static constexpr float verticesSmaller[9] = {
             -0.3f,  0.1f, 0.0f,
             0.0f,  -0.5f, 0.0f,
             0.3f,  0.1f, 0.0f
        };

        static constexpr float verticesRectSmaller[12] = {
             0.2f,  0.2f, 0.0f,  // top right
             0.2f, -0.2f, 0.0f,  // bottom right
            -0.2f, -0.2f, 0.0f,  // bottom left
            -0.2f,  0.2f, 0.0f   // top left
        };


        Triangle triangleSmaller(shaderRedTriangle, verticesSmaller);

        Rectangle rectangle(shaderGreen);
        Rectangle rectangleBlue(shaderBlue, verticesRectSmaller);

        while(window.shouldKeepAlive()) {
            window.clear();
            rectangle.draw();
            triangle.draw();
            triangleSmaller.draw();
            rectangleBlue.draw();
            window.display();
            window.pollEvents();
        }

    } catch(const GLException& ex) {
        std::cerr << ex << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
