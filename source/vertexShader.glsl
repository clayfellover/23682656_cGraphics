#version 330 core

layout(location = 0) in vec3 position;    
layout(location = 1) in vec3 colour;     
layout(location = 2) in vec2 uv;          
layout(location = 3) in vec3 normal;      
layout(location = 4) in vec3 tangent;     

out vec2 UV;                
out vec3 vertexColour;      
out vec3 FragPos;           
out mat3 TBN;               

uniform mat4 model;          
uniform mat4 view;           
uniform mat4 projection;     

void main() {
    // Model-view matrix
    mat4 MV = view * model;

    // Calculate the inverse of the 3x3 part of MV (for directions only)
    mat3 invMV = transpose(inverse(mat3(MV)));

    // Transform tangent and normal into view space
    vec3 t = normalize(invMV * tangent);
    vec3 n = normalize(invMV * normal);

    // Re-orthogonalize tangent and compute bitangent
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(n, t);

    // Create TBN matrix to convert from view space to tangent space
    TBN = transpose(mat3(t, b, n));

    // Outputs
    FragPos = vec3(MV * vec4(position, 1.0));
    UV = uv;
    vertexColour = colour;

    // Final position for rasterization
    gl_Position = projection * vec4(FragPos, 1.0);
}
















