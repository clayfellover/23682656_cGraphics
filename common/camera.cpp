#include <common/camera.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

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

void Camera::updateCameraVectors()
{
    // Convert the custom Quaternion to a glm::quat
    glm::quat q(orientation.x, orientation.y, orientation.z, orientation.w); 

    // Rotate the forward vector by the quaternion
    glm::vec3 newFront = glm::rotate(q, glm::vec3(0.0f, 0.0f, -1.0f)); 
    front = glm::normalize(newFront); 

    // Calculate the right and up vectors
    right = glm::normalize(glm::cross(front, worldUp)); 
    up = glm::normalize(glm::cross(right, front)); 
} 

void Camera::processInput(char key, float deltaTime, bool running)
{
    float velocity = speed * deltaTime;

    if (running) {
        velocity *= 2.0f;
    }

    // Convert the custom Quaternion to glm::quat
    glm::quat q(orientation.x, orientation.y, orientation.z, orientation.w);

    // Rotate the forward vector by the quaternion 
    glm::vec3 forward = glm::rotate(q, glm::vec3(0.0f, 0.0f, -1.0f));  // Forward (negative z)
    glm::vec3 right = glm::rotate(q, glm::vec3(1.0f, 0.0f, 0.0f));   // Right (positive x)
    glm::vec3 up = glm::rotate(q, glm::vec3(0.0f, 1.0f, 0.0f));      // Up (positive y)

    // Normalize vectors to prevent fast movement diagonally
    forward = glm::normalize(forward);
    right = glm::normalize(right);
    up = glm::normalize(up);

    // Move the camera using the rotated vectors
    if (key == 'W')
        eye += forward * velocity;  // Move forward along the forward vector
    if (key == 'S')
        eye -= forward * velocity;  // Move backward along the forward vector
    if (key == 'A')
        eye -= right * velocity;    // Move left along the right vector
    if (key == 'D')
        eye += right * velocity;    // Move right along the right vector

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

    targetOrientation = Quaternion(-pitch, yaw); 
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

void Camera::quaternionCamera()
{
    // Smoothly interpolate toward the target orientation
    orientation = Maths::SLERP(orientation, targetOrientation, 0.2f);

    // Convert custom Quaternion to glm::quat
    glm::quat glmQuat = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);

    // Convert to rotation matrix
    glm::mat4 rotationMatrix = glm::toMat4(glmQuat);

    // Create the view matrix
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -eye);
    view = rotationMatrix * translation;

    // Update direction vectors from rotation
    front = glm::normalize(glm::vec3(rotationMatrix[2] * -1.0f));
    right = glm::normalize(glm::vec3(rotationMatrix[0]));
    up = glm::normalize(glm::vec3(rotationMatrix[1]));

    // Projection matrix
    projection = glm::perspective(fov, aspect, near, far);
}


