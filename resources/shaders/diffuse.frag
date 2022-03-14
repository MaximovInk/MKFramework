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



// Gets the Texture Units from the main function
uniform sampler2D diffuse0;
uniform sampler2D specular0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;


vec4 pointLight()
{	
	// used in two variables so I calculate it here to not have to do it twice
	vec3 lightVec = lightPos - crntPos;

	// intensity of light with respect to distance
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	vec4 diffuse0Sampled = texture(diffuse0, texCoord);
	vec3 new = diffuse0Sampled.rgb * (diffuse * inten + ambient);
	diffuse0Sampled = vec4(new,diffuse0Sampled.a);

	return (diffuse0Sampled + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 direcLight()
{
	// ambient lighting
	float ambient = 0.35f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(-0.6f, 0.65f, 0.4f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.1f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 2);
	float specular = specAmount * specularLight;

	
	vec4 diffuse0Sampled = texture(diffuse0, texCoord) ;


	vec3 color =  diffuse0Sampled.rgb;
    if(color.r == color.g && color.r == color.b)
    {
        color.r = color.r/2.0;
        color.g = color.g*1.5;
        color.b = color.b/2.0;
    }

	vec3 new = color * (diffuse*0.8 + ambient);
	diffuse0Sampled = vec4(new,diffuse0Sampled.a);
	return (diffuse0Sampled + texture(specular0, texCoord).r * specular) * lightColor;
	

	//return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
	// controls how big the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	// calculates the intensity of the crntPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	vec4 diffuse0Sampled = texture(diffuse0, texCoord);
	vec3 new = diffuse0Sampled.rgb * (diffuse * inten + ambient);
	diffuse0Sampled = vec4(new,diffuse0Sampled.a);

	return (diffuse0Sampled + texture(specular0, texCoord).r * specular * inten) * lightColor;
}
void main()
{
	//FragColor = texture(diffuse0, texCoord);
	FragColor = direcLight();
}