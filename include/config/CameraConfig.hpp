#ifndef CAMERA_CONFIG_HPP
#define CAMERA_CONFIG_HPP

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

struct CameraConfig {
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;

    CameraConfig(const float& YAW = -90.0f, const float& PITCH = 0.0f, const float& SPEED = 2.5f, const float& SENSITIVITY = 0.001f, const float& ZOOM = 45.0f);
};

#endif // !CAMERA_CONFIG_HPP
