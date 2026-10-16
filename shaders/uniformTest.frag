#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // This variable is set in the OpenGL code

void main()
{
    FragColor = ourColor;
}