#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/GLException.hpp"
#include "core/GLWindow.hpp"
#include "core/Shader.hpp"
#include "core/ShaderProgram.hpp"
#include "drawable/TexturedCube.hpp"
#include "drawable/TexturedRectangle.hpp"
#include "drawable/Triangle.hpp"
#include "drawable/Rectangle.hpp"

void processInput(GLFWwindow *window, float& mixValue);

int main() {

    try {
        GLWindow window(800, 800, "Refactored OpenGL - Going 3D");

        // Create our Projection matrixes
        glm::mat4 model = glm::mat4(1.0f);                                              // Model translates from local to global
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 

        glm::mat4 view = glm::mat4(1.0f);                                               // View translates global to "camera"
        // translating the scene reverse of where we want to move our camera (camera stays "fixed", we move the scene)
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

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

        // Prevent weird overlap of rotation cube-faces
        glEnable(GL_DEPTH_TEST);  

        // Main render loop
        while(window.shouldKeepAlive()) {
            processInput(window.get(), mixValue);
            texture3D.setFloat("mixValue", mixValue);
            window.clear();

            model = glm::rotate(model, (float)glfwGetTime() * 0.0001f, glm::vec3(0.5f, 1.0f, 0.0f));

            int modelLoc = glGetUniformLocation(texture3D.getID(), "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            int viewLoc = glGetUniformLocation(texture3D.getID(), "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            int projectionLoc = glGetUniformLocation(texture3D.getID(), "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            /*rectangle.draw();*/
            cube.draw();
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

void processInput(GLFWwindow *window, float& mixValue) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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
