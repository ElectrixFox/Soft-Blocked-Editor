#include "Buffer.h"

unsigned int CreateVBO(float vertices[], unsigned int n)
{
unsigned int vbo;
glGenBuffers(1, &vbo);  // generate the buffer
glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the vbo to type of array buffer to be targetted by buffer data
glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n, vertices, GL_STATIC_DRAW);  // targets the buffer of type GL_ARRAY_BUFFER and then sets its data

return vbo;
}

unsigned int CreateIBO(unsigned int indices[], unsigned int n)
{
unsigned int ibo;
glGenBuffers(1, &ibo);  // generating the buffer
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // binding the buffer to an element array buffer
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * n, indices, GL_STATIC_DRAW);    // writing the indices to it

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // binding the buffer

return ibo;
}

unsigned int CreateVAO()
{
unsigned int vao;

glGenVertexArrays(1, &vao); // only bind one array and generate the arrays
glBindVertexArray(vao); // bind the array to be used

return vao;
}

void AddToVertexLayout(VAOLayout& layout, unsigned int size) { layout.sizes.push_back(size); }

VAOLayout CreateVertexLayout(unsigned int sizes[], unsigned int bufflen, unsigned int n)
{
VAOLayout layout;
for (int i = 0; i < n; i++)
    layout.sizes.push_back(sizes[i]);
layout.bufflen = bufflen;

return layout;
}

void InitialiseVertexLayout(VAOLayout layout)
{
unsigned int stride = 0;
for(int i = 0; i < layout.sizes.size(); i++)
    {
    glVertexAttribPointer(i, layout.sizes[i], GL_FLOAT, GL_FALSE, layout.bufflen * sizeof(float), (void*)(stride * sizeof(float))); // adding the pointer
    glEnableVertexAttribArray(i);   // attributing the data.
    stride += layout.sizes[i];
    }
}

void BindVAO(unsigned int vao)
{
glBindVertexArray(vao); // simple bind
}

void BindIBO(unsigned int ibo)
{
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // binding the buffer
}

void BindVBO(unsigned int vbo)
{
glBindBuffer(GL_ARRAY_BUFFER, vbo); // binding the buffer to the array buffer
}