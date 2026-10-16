#version 330 core
layout (location = 0) in vec3 aPos;

uniform vec3 offset;

void main()
{
    vec3 finalPosition = aPos + offset;
    gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, 1.0);
}