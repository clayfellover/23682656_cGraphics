#version 330 core

in vec2  UV;
in vec3  FragPos;
in vec3  Normal;
out vec4 FragColour;

uniform sampler2D textureMap;

// Ambient + Specular light
uniform vec3 ambientLightColour;
uniform vec3 specularLightColour;
uniform float shininess;

// Camera
uniform vec3 viewPos;

// Directional light
uniform vec3 lightDir;

void main() {
    // Sample the base texture colour
    vec3 texCol = texture(textureMap, UV).rgb;

    // Ambient 
    vec3 ambient = ambientLightColour * texCol;

    // Diffuse
    vec3 norm = normalize(Normal);
    // lightDir in your code is the *direction the light comes from*, so we negate it here
    vec3 L = normalize(-lightDir);
    float diff = max(dot(norm, L), 0.0);
    vec3 diffuse = diff * texCol;

    // Specular
    vec3 V = normalize(viewPos - FragPos);
    vec3 R = reflect(-L, norm);
    float spec = pow(max(dot(V, R), 0.0), shininess);
    vec3 specular = spec * specularLightColour;

    // Combine
    vec3 result = ambient + diffuse + specular;
    FragColour = vec4(result, 1.0);
}








