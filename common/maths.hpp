#pragma once

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtx/io.hpp>

class Maths
{
public:
    // Transformation stuff
    static glm::mat4 translate(const glm::vec3& v);

    // Scaling stuff
    static glm::mat4 scale(const glm::vec3& v); 

    // Rotation stuff
    static glm::mat4 rotate(float angle);
};
