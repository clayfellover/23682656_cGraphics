#include "light.hpp"

// initialise light with both ambient and specular colours
Light::Light(const glm::vec3& ambientColour, const glm::vec3& specularColour, float shininess)
    : ambientColour(ambientColour), specularColour(specularColour), shininess(shininess) {
}

// get for ambient light 
glm::vec3 Light::getAmbientColour() const {
    return ambientColour;
}

// set for ambient light 
void Light::setAmbientColour(const glm::vec3& colour) {
    ambientColour = colour;
}

// get for specular light 
glm::vec3 Light::getSpecularColour() const {
    return specularColour;
}

// set for specular light 
void Light::setSpecularColour(const glm::vec3& colour) {
    specularColour = colour;
}

// get for shininess
float Light::getShininess() const {
    return shininess;
}

// set for shininess
void Light::setShininess(float s) {
    shininess = s;
}



