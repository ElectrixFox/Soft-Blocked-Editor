#version 430 core
out vec4 FragColor;

uniform vec4 colour;

void main()
{
FragColor = vec4(colour);
}