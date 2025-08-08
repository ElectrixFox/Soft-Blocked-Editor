#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D intexture;

void main()
{
vec4 sampled = vec4(1.0, 1.0, 1.0, texture(intexture, TexCoord).r);
FragColor = vec4(1.0, 0.0, 0.0, 1.0) * sampled;
}