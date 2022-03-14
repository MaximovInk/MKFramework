#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 crntPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;

uniform sampler2D diffuse0;


void main()
{
	FragColor = texture(diffuse0, texCoord);
	//FragColor = vec4(vec3(linearizeDepth(gl_FragCoord.z)/far), 1.0f);

	
}