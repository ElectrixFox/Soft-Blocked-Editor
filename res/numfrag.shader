#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D intexture;
uniform sampler2D texture2;

void main()
{
FragColor = mix(texture(intexture, TexCoord), texture(texture2, TexCoord), 0.2);
}