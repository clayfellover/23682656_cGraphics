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
    // convert to glm::quat
    glm::quat q(orientation.x, orientation.y, orientation.z, orientation.w); 

    // rotate the forward vector by the quaternion
    glm::vec3 newFront = glm::rotate(q, glm::vec3(0.0f, 0.0f, -1.0f)); 
    front = glm::normalize(newFront); 

    // calculate the right and up vectors
    right = glm::normalize(glm::cross(front, worldUp)); 
    up = glm::normalize(glm::cross(right, front)); 
} 

void Camera::processInput(char key, float deltaTime, bool running)
{
    float velocity = speed * deltaTime;

    if (running) {
        velocity *= 2.0f;
    }

    // convert to glm::quat
    glm::quat q(orientation.x, orientation.y, orientation.z, orientation.w);

    // rotate the forward vector by the quaternion 
    glm::vec3 forward = glm::rotate(q, glm::vec3(0.0f, 0.0f, -1.0f));  
    glm::vec3 right = glm::rotate(q, glm::vec3(1.0f, 0.0f, 0.0f));   
    glm::vec3 up = glm::rotate(q, glm::vec3(0.0f, 1.0f, 0.0f));   

    // normalize vectors to prevent fast movement diagonally
    forward = glm::normalize(forward);
    right = glm::normalize(right);
    up = glm::normalize(up);

    // move the camera using the rotated vectors
    if (key == 'W')
        eye += forward * velocity;
    if (key == 'S')
        eye -= forward * velocity;  
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
    // interpolate towards target orientation
    orientation = Maths::SLERP(orientation, targetOrientation, 0.2f);

    // convert to glm::quat
    glm::quat glmQuat = glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);

    // convert to rotation matrix
    glm::mat4 rotationMatrix = glm::toMat4(glmQuat);

    // create the view matrix
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -eye);
    view = rotationMatrix * translation;

    // update direction vectors from rotation
    front = glm::normalize(glm::vec3(rotationMatrix[2] * -1.0f));
    right = glm::normalize(glm::vec3(rotationMatrix[0]));
    up = glm::normalize(glm::vec3(rotationMatrix[1]));

    // projection matrix
    projection = glm::perspective(fov, aspect, near, far);
}


