#version 330 core

out vec4 FragColor;

uniform sampler2D diffuse0;

in vec2 texCoords;

void main()
{
    vec3 color =  texture(diffuse0, texCoords).rgb;
    if(color.r == color.g && color.r == color.b)
    {
        color.r = color.r/2.0;
        color.g = color.g*1.5;
        color.b = color.b/2.0;
    }

    FragColor = vec4(color, 1.0f);
};