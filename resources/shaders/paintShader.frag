#version 330 core

out vec4 FragColor;

uniform sampler2D diffuse0;

in vec2 texCoords;

void main()
{
    vec3 color =  texture(diffuse0, texCoords).rgb;
    if(color.r == color.g && color.b == color.g)
        FragColor = vec4(color,0.0);    
    else
        FragColor = vec4(color, 1.0);
};