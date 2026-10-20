#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    //We want to pass the fragment position and normal to the fragment shader for lighting calculations
    //All of this is done in world space, so we need to transform the vertex position and normal to world space using the model matrix
    FragPos = vec3(model * vec4(aPos, 1.0));
    // Normal = aNormal;
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
}
