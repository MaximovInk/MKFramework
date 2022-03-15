#version 330 core

out vec4 FragColor;

uniform sampler2D diffuse0;

in vec2 texCoords;

void main()
{
    vec3 color =  texture(diffuse0, texCoords).rgb;

    FragColor = vec4(color, 1.0f);
};