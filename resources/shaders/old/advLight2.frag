#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

uniform sampler2D diffuse0;
uniform sampler2D normal0;
uniform samplerCube depthMap0;
uniform sampler2D depth0;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

float ShadowCalculation(vec3 fragPos)
{

 float far_plane = 25.0;

    // ѕолучаем вектор между положением фрагмента и положением источника света
    vec3 fragToLight = fragPos - lightPos;
	
    // »спользуем полученный вектор дл€ выборки из карты глубины    
    float closestDepth = texture(depthMap0, fragToLight).r;
	
    // ¬ данный момент значени€ лежат в диапазоне [0,1]. ѕреобразуем их обратно к исходным значени€м
    //closestDepth *= ;
	
    // “еперь получим текущую линейную глубину как длину между фрагментом и положением источника света
    float currentDepth = length(fragToLight)/far_plane;
	
    // “еперь проводим проверку на нахождение в тени
    float bias = 0.015; // мы используем гораздо большее теневое смещение, так как значение глубины теперь находитс€ в  диапазоне [near_plane, far_plane]
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;        
    
	// отладка - отображение значений переменной closestDepth (дл€ визуализации кубической карты глубины)
    // FragColor = vec4(vec3(closestDepth / far_plane), 1.0);    
        
    return shadow;
}

uniform float height_scale;
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
const float minLayers = 8.0;
const float maxLayers = 32.0;
float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));
  float layerDepth = 1.0 / numLayers;
 
    // √лубина текущего сло€
    float currentLayerDepth = 0.0;
 
    // ¬еличина сдвига текстурных координат дл€ каждого сло€ (из вектора P)
    vec2 P = viewDir.xy * 0.1; 
    vec2 deltaTexCoords = P / numLayers;
vec2 currentTexCoords = texCoords;
float currentDepthMapValue = texture(depth0, currentTexCoords).r;
  
while(currentLayerDepth < currentDepthMapValue)
{
    // —двигаем текстурные координаты вдоль направлени€ вектора P
    currentTexCoords -= deltaTexCoords;
 
    // ѕолучаем значение глубины из карты глубины дл€ текущих текстурных координат
    currentDepthMapValue = texture(depth0, currentTexCoords).r;  
 
    // ѕолучаем значение глубины следующего сло€
    currentLayerDepth += layerDepth;  

    

}
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depth0, prevTexCoords).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords; 
} 

void gBufferTest(){
 vec3 color = texture(diffuse0, fs_in.TexCoords).rgb;
  FragColor = vec4(color, 1.0);
}

void main()
{   gBufferTest();
    return;

/*
if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    discard;
   
*/
   
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = ParallaxMapping(fs_in.TexCoords,viewDir);


    vec3 color = texture(diffuse0, texCoords).rgb;
    
    vec3 normal = texture(normal0, texCoords).rgb;
     normal = normalize(normal * 2.0 - 1.0); 
   
    vec3 lightColor = vec3(1);
	
    // ‘онова€ составл€юща€
    vec3 ambient = 0.1 * color;
	
    // ƒиффузна€ составл€юща€
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
	
    // ќтраженна€ составл€юща€
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;    
	
    // ¬ычисл€ем тень
    float shadow =  1.0-ShadowCalculation(fs_in.FragPos);             
    vec3 lighting = (ambient + (shadow) * (diffuse + specular)) * color;    
    
    FragColor = vec4(lighting, 1.0);
     
}