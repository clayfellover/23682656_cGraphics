#pragma once

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <common/maths.hpp>

class Camera
{
public:
    // projection parameters
    float fov = Maths::radians(100.0f);
    float aspect = 1024.0f / 768.0f;
    float near = 0.2f;
    float far = 100.0f;

    // camera vectors
    glm::vec3 eye;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // transformation matrices
    glm::mat4 view;
    glm::mat4 projection;

    // movement + look parameters
    float speed = 5.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float sensitivity = 0.1f;

    // jumping / physics
    float verticalVelocity = 0.0f;
    float gravity = -20.0f;
    bool isGrounded = true;
    float jumpStrength = 8.0f;

    // Quaternion camera
    Quaternion orientation = Quaternion(pitch, yaw); 
    Camera(const glm::vec3 eye, const glm::vec3 target);

    // view/projection matrices
    void quaternionCamera(); 

    // wasd
    void processInput(char key, float deltaTime, bool running);

    // mouse movement input
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // front/right/up from yaw & pitch
    void updateCameraVectors();

    // jump
    void updatePhysics(float deltaTime);

    // quaternion camera
    void quaternionCamera(); 
};




