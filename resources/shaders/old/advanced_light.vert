#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;



out vec2 TexCoords;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

//

uniform mat4 camMatrix;
uniform mat4 model;

uniform mat4 lightSpaceMatrix;

void main()
{
	vec3 crntPos = vec3(model * vec4(aPos, 1.0f));
    vs_out.FragPos = crntPos;
   //vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(aPos, 1.0);

	gl_Position = camMatrix* vec4(crntPos, 1.0);
   
}