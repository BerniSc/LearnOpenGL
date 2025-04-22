#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "config/CameraConfig.hpp"

#include <cstddef>
#include <glm/glm.hpp>

class Camera {
    private:
        CameraConfig config;

        // Vectors to describe our Camera-Space System
        // Right-Hand-Coordinatesystem -> pos z-Axis is out of screen
        glm::vec3 cameraPosition;
        glm::vec3 cameraDirection;
        glm::vec3 cameraRight;
        glm::vec3 cameraUp;
        glm::vec3 worldUp;
        
        // Euler angles
        float yaw;
        float pitch;

        // Alternative -> Specify a LookAt target
        glm::vec3 cameraTarget;

        void updateCameraVectors();

    public:   
        Camera(glm::vec3 cameraPosition, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), CameraConfig config = CameraConfig());

        glm::mat4 getViewMatrix() const;

        glm::mat4 getProjectionMatrix(std::size_t windowWidth, std::size_t windowHeight, float near = 0.1f, float far = 100.0f) const;

        // Processing Handlers
        //
        // processes input received from any keyboard-like input system.
        // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(CameraMovement direction, float deltaTime);
        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(float yOffset);

        void pointCameraAt(const glm::vec3& target);
        void setCameraPosition(const glm::vec3& target);
};

#endif // !CAMERA_HPP
