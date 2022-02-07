#version 330 core

out vec4 FragColor;

uniform sampler2D diffuse0;

in vec2 texCoords;

void main()
{
    FragColor = vec4(texture(diffuse0, texCoords).rgb, 1.0f);
};