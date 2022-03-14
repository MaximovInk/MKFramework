


#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuse0;
uniform sampler2D shadowMap0;

uniform vec3 lightPos;
uniform vec3 viewPos;


float ShadowCalculation(vec4 fragPosLightSpace,float bias)
{
    // Выполняем деление перспективы
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
    // Преобразуем в диапазон [0,1]
    projCoords = projCoords * 0.5 + 0.5;
	
    // Получаем наиболее близкое значение глубины исходя из перспективы глазами источника света (используя диапазон [0,1] fragPosLight в качестве координат)
    float closestDepth = texture(shadowMap0, projCoords.xy).r; 
	
    // Получаем глубину текущего фрагмента исходя из перспективы глазами источника света
    float currentDepth = projCoords.z;
	
    // Проверяем, находится ли текущий фрагмент в тени
    
    //float shadow = currentDepth-bias > closestDepth  ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap0, 0);
for(int x = -1; x <= 1; ++x)
{
    for(int y = -1; y <= 1; ++y)
    {
        float pcfDepth = texture(shadowMap0, projCoords.xy + vec2(x, y) * texelSize).r; 
        shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
    }    
}
shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

        

    return shadow;
}

void main()
{          
   //FragColor = texture(diffuse0, fs_in.TexCoords);
    //FragColor = vec4(fs_in.TexCoords.x,fs_in.TexCoords.y,1, 1);
    //return;

    vec3 color = texture(diffuse0, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1);
	
    // Фоновая составляющая
    vec3 ambient = 0.3 * color;
	
    // Диффузная составляющая
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    //float bias = 0.005;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // Вычисляем тень
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace,bias);                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse)) * color;    
    
    FragColor = vec4(lighting, 1.0);
}
