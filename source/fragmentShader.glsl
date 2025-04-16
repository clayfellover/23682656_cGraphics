#version 330 core

// Input
in vec2 UV;

// Output
out vec3 colour;

// Uniforms
uniform sampler2D textureMap;

void main()
{
    // Sample the texture, and discard the alpha channel (using vec3)
    colour = vec3(texture(textureMap, UV).rgb);
}
