#version 330 core
out vec4 FragColor;

// in vec3 ourColor;
in vec2 TexCoord;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D texture0; //0
uniform sampler2D texture1; //1

void main()
{
    FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.4);
    FragColor = vec4(lightColor * objectColor, 1.0);
} 
