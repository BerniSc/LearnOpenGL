#include "core/Camera.hpp"

#include <glm/geometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**/
/*Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget) : cameraPosition(cameraPosition), cameraTarget(cameraTarget) {*/
/*    // Intuitively it would be camTarget - camPos, but we changed the order here to have the coordinates*/
/*    // point in the same direction as our pos z from the camera (instead of always having -z).*/
/*    // Effectively this is not the direction of the camera but rather the direct light-ray getting picked up by the camera*/
/*    cameraDirection = glm::normalize(cameraPosition - cameraTarget);*/
/**/
/*    // Generate our right vector by crossing our local cameradirection with the global "up" vector*/
/*    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);*/
/*    cameraRight = glm::normalize(glm::cross(up, cameraDirection));*/
/**/
/*    // Using cameraDirection and cameraRight we can get the cameraUp now*/
/*    cameraUp = glm::cross(cameraDirection, cameraRight);*/
/**/
/**/
/*    // With all these we can Generate a LookAt matrix -> Takes in arbitrary coordinate and transform it to our own "camera space"*/
/*}*/
/**/

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 worldUp, CameraConfig config) :
    config(config), cameraDirection(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(worldUp) {

    this->cameraPosition = cameraPosition;
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // calc the new front-Vector
    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    this->cameraDirection = glm::normalize(front);

    // Also recalculate the Right and Up Vecs -> Normalze them to abstract the movement speed for looking up or
    // down (makes values smaller -> slower movement)
    this->cameraRight = glm::normalize(glm::cross(cameraDirection, worldUp));
    this->cameraUp = glm::normalize(glm::cross(cameraRight, cameraDirection));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(cameraPosition, cameraPosition + cameraDirection, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix(std::size_t windowWidth, std::size_t windowHeight, float near, float far) const {
    return glm::perspective(glm::radians(this->config.zoom), (float)windowWidth / (float)windowHeight, near, far);
}

void Camera::pointCameraAt(const glm::vec3& target) {
    glm::vec3 direction = glm::normalize(target - cameraPosition);

    // Calculate yaw and pitch from direction vector
    pitch = glm::degrees(std::asin(direction.y));
    yaw = glm::degrees(std::atan2(direction.z, direction.x));

    updateCameraVectors();
}

void Camera::setCameraPosition(const glm::vec3& target) {
    this->cameraPosition = target;
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = this->config.speed * deltaTime;

    if(direction == FORWARD)
        cameraPosition += cameraDirection * velocity;
    if(direction == BACKWARD)
        cameraPosition -= cameraDirection * velocity;
    if(direction == LEFT)
        cameraPosition -= cameraRight * velocity;
    if(direction == RIGHT)
        cameraPosition += cameraRight * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= this->config.sensitivity;
    yOffset *= this->config.sensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // when pitch is out of bounds, screen should not get flipped
    if(constrainPitch) {
        if(pitch > 89.0f)
            pitch = 89.0f;
        if(pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
    this->config.zoom -= yOffset;
    if(this->config.zoom < 1.0f)
        this->config.zoom = 1.0f;
    if(this->config.zoom > 45.0f)
        this->config.zoom = 45.0f;
}

