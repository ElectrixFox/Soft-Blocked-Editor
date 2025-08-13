#shader Vertex
#version 430 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
gl_Position = transpose(model * view * projection) * vec4(pos, 1.0);
}

#shader Fragment
#version 430 core
out vec4 FragColor;

uniform vec4 colour;

void main()
{
FragColor = vec4(colour);
}