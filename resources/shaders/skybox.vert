
/*
#version 330 core
layout (location = 0) in vec3 aPos;
 
out vec3 TexCoords;
 
uniform mat4 camMatrix;
 
void main()
{
    TexCoords = aPos;
    gl_Position =  vec4(aPos, 1.0);
} 

*/
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
