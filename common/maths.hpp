#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

class Quaternion
{

public:
    float w, x, y, z;
    Quaternion();
    // Constructors
    Quaternion(const float w, const float x, const float y, const float z);
    glm::mat4 matrix(); 

    // quaternion stuf
    Quaternion(const float pitch, const float yaw); 

};

class Maths
{
public:
    // Transformation stuff
    static glm::mat4 translate(const glm::vec3& v);

    // Scaling stuff
    static glm::mat4 scale(const glm::vec3& v); 

    // Rotation stuff
    static glm::mat4 rotate(const float& angle, glm::vec3 v);

    // radians!!
    static float radians(float degrees);

    // SLERP
    static Quaternion SLERP(Quaternion q1, Quaternion q2, float t);
};
