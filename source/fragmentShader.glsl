#version 330 core

in vec2 UV;
in vec3 FragPos;
in vec3 vertexColour;
in mat3 TBN;

out vec4 FragColour;

// Texture samplers
uniform sampler2D textureMap;  
uniform sampler2D normalMap; 
uniform sampler2D specularMap;  

// Lighting
uniform vec3 ambientLightColour;
uniform vec3 specularLightColour;
uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform float shininess;

// Camera
uniform vec3 viewPos;

void main() {
    // Base albedo from texture
    vec3 texCol = texture(textureMap, UV).rgb;

    // Get normal from normal map and transform to world/view space
    vec3 normalSample = texture(normalMap, UV).rgb;
    vec3 tangentNormal = normalize(normalSample * 2.0 - 1.0);
    vec3 norm = normalize(TBN * tangentNormal);

    // Specular strength from specular map
    float specularStrength = texture(specularMap, UV).r;

    // Ambient component
    vec3 ambient = ambientLightColour * texCol;

    // Directional light
    vec3 lightDir = normalize(-lightDirection); 

    // Diffuse component
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour * texCol;

    // Specular component (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularStrength * specularLightColour;

    // Final color
    vec3 result = ambient + diffuse + specular;
    FragColour = vec4(result, 1.0);
}


















