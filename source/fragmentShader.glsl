#version 330 core

out vec4 FragColor;

in vec2 TexCoord;  // Texture coordinates passed from vertex shader

uniform sampler2D texture1;  // Single texture

void main()
{
    // Sample the texture
    FragColor = texture(texture1, TexCoord);
}
