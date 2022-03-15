#version 400 core
out vec4 FragColor;
 
in vec3 TexCoords;
 
uniform samplerCube skybox;
 
void main()
{   
    float max = 5.0;

    vec4 color =  texture(skybox, TexCoords);
    float y = gl_FragCoord.y;
    float x = gl_FragCoord.x;

    color = mix(color,vec4(0,1,1,1), (1080-y)/(3000));
    color = mix(color,vec4(1,0.5,0,1), (y)/(3000));

    FragColor = color;
    //FragColor = texture(skybox, TexCoords);

    //FragColor = vec4(1,1,1,1);
}