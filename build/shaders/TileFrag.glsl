#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main()
{
    // Sample the texture at the texture coordinate and output the color
    FragColor = texture(texture1, TexCoord);
}
