#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
    gl_Position = projection * model * view * vec4(aPos, 1.0);
    TexCoord = aTexCoord; // Pass the texture coordinate to the fragment shader
}
