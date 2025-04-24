#include <common/camera.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)
{
    eye = Eye;
    target = Target;
}

void Camera::calculateMatrices()
{
    // view matrix
    view = glm::lookAt(eye, target, worldUp);

    // projection matrix
    projection = glm::perspective(fov, aspect, near, far);
}

// rotate the camera around the worldUp axis based on pitch and yaw
void Camera::rotateCamera(float pitch, float yaw)
{
    // rotation matrix for yaw
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(yaw), worldUp);

    // apply the rotation to the target vector (camera's look-at point)
    target = glm::vec3(rotationMatrix * glm::vec4(target, 1.0f));

    // recalculate the view matrix after the rotation
    calculateMatrices();
}
