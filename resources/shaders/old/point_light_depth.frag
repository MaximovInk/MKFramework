#version 330 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    
    // ѕриводим значение к диапазону [0; 1] путем делени€ на far_plane
    lightDistance = lightDistance / far_plane;
    
    // «аписываем его в качестве измененной глубины
    gl_FragDepth = lightDistance;
}