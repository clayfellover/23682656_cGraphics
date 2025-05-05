#pragma once

#include <glm/glm.hpp>

class Light {
public:
    // ambient and specular colours + shiny
    Light(
        const glm::vec3& ambientColour = glm::vec3(0.2f, 0.2f, 0.2f),
        const glm::vec3& specularColour = glm::vec3(1.0f, 1.0f, 1.0f),
        float             shininess = 32.0f
    );

    // ambient getset
    glm::vec3 getAmbientColour() const;
    void      setAmbientColour(const glm::vec3& colour);

    // specular getset
    glm::vec3 getSpecularColour() const;
    void      setSpecularColour(const glm::vec3& colour);

    // shiny getset
    float     getShininess() const;
    void      setShininess(float s);

private:
    glm::vec3 ambientColour;
    glm::vec3 specularColour;
    float     shininess;
};


