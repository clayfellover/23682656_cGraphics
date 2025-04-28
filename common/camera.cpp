#include <common/camera.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)
{
    eye = Eye;
    front = glm::normalize(Target - Eye);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    updateCameraVectors();

    verticalVelocity = 0.0f;
    gravity = -9.81f;
    jumpStrength = 5.0f;
    isGrounded = true;
}

void Camera::calculateMatrices()
{
    view = glm::lookAt(eye, eye + front, up);
    projection = glm::perspective(fov, aspect, near, far);
}

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::processInput(char key, float deltaTime, bool running)
{
    float velocity = speed * deltaTime;

    if (running) {
        velocity *= 2.0f;
    }

    if (key == 'W')
        eye += front * velocity;
    if (key == 'S')
        eye -= front * velocity;
    if (key == 'A')
        eye -= right * velocity;
    if (key == 'D')
        eye += right * velocity;

    if (key == ' ')
    {
        if (isGrounded)
        {
            verticalVelocity = jumpStrength;
            isGrounded = false;
        }
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updatePhysics(float deltaTime)
{
    if (!isGrounded)
    {
        verticalVelocity += gravity * deltaTime;
        eye.y += verticalVelocity * deltaTime;

        if (eye.y <= 0.0f)
        {
            eye.y = 0.0f;
            verticalVelocity = 0.0f;
            isGrounded = true;
        }
    }
}



