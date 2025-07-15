#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D intexture;

void main()
{
vec4 texColour = texture(intexture, TexCoord * 20.0);
FragColor = vec4(texColour.rgb, 0.0);   
}