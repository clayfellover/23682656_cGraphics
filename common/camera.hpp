#pragma once

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <common/maths.hpp>

class Camera
{
public:
    // Projection parameters
    float fov = Maths::radians(100.0f);
    float aspect = 1024.0f / 768.0f; 
    float near = 0.2f;
    float far = 100.0f;

    // Camera vectors
    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Transformation matrices
    glm::mat4 view;
    glm::mat4 projection;

    // Constructor
    Camera(const glm::vec3 eye, const glm::vec3 target);

    // Methods
    void calculateMatrices();  
    void rotateCamera(float pitch, float yaw); 
};

