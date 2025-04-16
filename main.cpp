// main.cpp

#include <cstddef>
#include <cstdint>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

struct MyGLException {
    const std::string& reason;

    MyGLException(std::string exceptionContent) : reason(exceptionContent) { }
};

std::ostream& operator<<(std::ostream& os, const MyGLException& ex) {
    os << ex.reason << "\n";  
    return os;
}

class MyGLTriangle {
    // Data sent over to the GPU
    unsigned int vertexBufferObject, vertexBufferArray;
    // Shaders
    unsigned int vertexShader, fragmentShader;
    unsigned int shaderProgram;

    int success = 1;

    // TODO Fix this to be not const
    static constexpr float vertices[] = {
         0.5f,  0.5f, 0.0f,                 // top right
         0.5f, -0.5f, 0.0f,                 // bottom right
        -0.5f, -0.5f, 0.0f,                 // bottom left
    };

    struct RAIICleanup {
        MyGLTriangle* parentTriangle;

        RAIICleanup(MyGLTriangle* parentTriangle) : parentTriangle(parentTriangle) {
            glBindBuffer(GL_ARRAY_BUFFER, parentTriangle->vertexBufferObject); 
        }

        ~RAIICleanup() {
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
        }
    };


    // For now do not change the Triangle -> Just "Fire once" and forget
    void bufferDataStatic() {
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }

    void compileShader(const char* shaderCode, unsigned int& shader) {
        glShaderSource(shader, 1, &shaderCode, NULL);
        glCompileShader(shader);

        // Check for errors
        char errorMessage[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 512, NULL, errorMessage);
            std::cout << "CompileShader failed: " << errorMessage << std::endl;
            throw MyGLException("CompileShader failed");
        }
        success = 1;
    }

    // Creating shaderProgram to bundle the compiled shaders
    // Chainging -> I.E. using the output of one as the input of other (|)
    void applyShader() {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // check for linking errors
        char errorMessage[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, errorMessage);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << errorMessage << std::endl;
        }
        success = 1;

        glUseProgram(shaderProgram);

        // Cleanup unused resources
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    public:
        void draw() {
            glBindVertexArray(vertexBufferArray);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        void initTriangle() {
            // Create the Buffer in the GPU and then bin it for our configuration (unbind after use via RAII
            glGenVertexArrays(1, &vertexBufferArray);
            glGenBuffers(1, &vertexBufferObject);
            glBindVertexArray(vertexBufferArray);
            RAIICleanup raiiBind(this);

            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            compileShader(vertexShaderSource, vertexShader);
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            compileShader(fragmentShaderSource, fragmentShader);

            applyShader();
        
            bufferDataStatic();

            // Tell GPU how to map our Vertices (Data to Actual used Data) :)
            // 0 comes from our specifing the position0 in shadercode
            // Automagically uses the last bound VBO (Last bond to GL_ARRAY_BUFFER 
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
            glEnableVertexAttribArray(0);
            // Unbind again TODO  Use RAII
            glBindBuffer(GL_ARRAY_BUFFER, 0); 
            glBindVertexArray(0);
        }

        ~MyGLTriangle() {
            glDeleteVertexArrays(1, &vertexBufferArray);
            glDeleteBuffers(1, &vertexBufferObject);
            glDeleteProgram(shaderProgram);
        }
};

class MyGLWindow {
    private:
        std::uint16_t width, height;
        std::string windowTitle;

        // Window
        GLFWwindow* window;


        // Init the Window using version 3.3 with Core Mode
        void initMainWindow() {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
            if(window == NULL) {
                std::cerr << "Failed to create GLFW window\n";
                glfwTerminate();
                throw MyGLException("Failed to create GLFW window\n");
            }

            // Init the Window
            glfwMakeContextCurrent(window);

            // Init the GLAD Loader to setup the correct addresses 
            if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                std::cerr << "Failed to initialize GLAD\n";
                throw MyGLException("Failed to initialize GLAD\n");
            }

            glViewport(0, 0, width, height);
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        }

    public:
        MyGLWindow(const std::uint16_t& width, const std::uint16_t& height, const std::string& windowTitle) :
            width(width), height(height), windowTitle(windowTitle) {
            
            initMainWindow();
        }

        ~MyGLWindow() {

        }

        GLFWwindow* getID() {
            return this->window;
        }

        bool shouldKeepAlive() {
            return !glfwWindowShouldClose(window);
        }

        void drawElements(MyGLTriangle* _try) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            _try->draw();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

};


int main() {


    MyGLWindow myWindow(800, 800, "Test");
    MyGLTriangle myTriangle;
    myTriangle.initTriangle();

    while(myWindow.shouldKeepAlive()) {

        /*processInput(myWindow.getID());*/
        myWindow.drawElements(&myTriangle);
    }

    return 0;
/*
    // Store the following configuration in a VAO (Vertex Array Object)
    // Benefit: can just "recall" this config by reusing the object
    // Contains the VBO etc
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);  

    glBindVertexArray(VAO);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  
    // Create Buffer -> Sending Data over to Graphics Card
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create EBO To share some of draw the vertices as a square using some of the same edges -> ie giving indices that can change the order drawn
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Create Shader to display the structure of the sent data 
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check for Compile success :-=)
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Shader compilation Failed\n" << infoLog << std::endl;
    }

    // Create shader to determine the color of the displayed Data
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Shader compilation Failed\n" << infoLog << std::endl;
    }


    // Creating shaderProgram to bundle the compiled shaders
    // Chainging -> I.E. using the output of one as the input of other (|)
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader Linking Failed\n Sucks to be you :-)" << infoLog << std::endl;
    }

    // Tell GraphicsCard to use the shaderProg from now on and clean up the trash
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);  

    // Tell GPU how to map our Vertices (Data to Actual used Data) :)
    // 0 comes from our specifing the position0 in shadercode
    // Automagically uses the last bound VBO (Last bond to GL_ARRAY_BUFFER 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    uint32_t counter = 0;
    bool toggle = false;
    while(!glfwWindowShouldClose(window)) {
        if(++counter >= 1'000) {
            if((toggle = !toggle)) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            counter = 0;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);

        //glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    */
    return 0;
}

