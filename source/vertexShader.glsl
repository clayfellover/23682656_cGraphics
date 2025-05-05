#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 normal;

out vec2 UV;
out vec3 vertexColor;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // World-space position of the fragment
    FragPos = vec3(model * vec4(position, 1.0));
    
    // Correctly transform normals by the inverse-transpose of the model
    Normal = mat3(transpose(inverse(model))) * normal;

    UV          = uv;
    vertexColor = color;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}





