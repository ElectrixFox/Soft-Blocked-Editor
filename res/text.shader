#shader Vertex
#version 430 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texcoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;

void main()
{
gl_Position = transpose(model * view * projection) * vec4(pos.x, pos.y, 0.0, 1.0);
TexCoord = texcoord;
}

#shader Fragment
#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D intexture;

void main()
{
vec4 sampled = vec4(1.0, 1.0, 1.0, texture(intexture, TexCoord).r);
vec3 txcol = vec3(0.0, 0.0, 0.0);
FragColor = vec4(txcol, 1.0) * sampled;
}