#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
 
uniform sampler2D screen0;
 uniform float exposure;

 void HDR(){
 


const float gamma = 2.4;
    vec3 hdrColor = texture(screen0, TexCoords).rgb;
  
    // Алгоритм тональной компрессии Рейнхарда
    float exposure = 1.0;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
 
    // Гамма-коррекция
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
 }

void main()
{ 
HDR();
   /*
   //FragColor = texture(screen0, TexCoords);
    // FragColor = vec4(vec3(1.0 - texture(screen0, TexCoords)), 1.0);

   FragColor = vec4(texture(screen0, TexCoords).rgb,1.0);
   //FragColor = vec4(1.0);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
   */
}