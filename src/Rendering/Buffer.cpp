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

void AddToVertexLayout(VAOLayout* layout, unsigned int size)
{
void* nptr = malloc(sizeof(unsigned int*) * (*layout).number); // allocating the new memory
nptr = realloc((*layout).sizes, sizeof(unsigned int) * ((*layout).number + 1));  // making the array bigger
(*layout).sizes = nptr;    // setting the old array to the new bigger one
(*layout).sizes[(*layout).number] = size; // setting the new size element
(*layout).number++;    // increase the number of elements in the whole layout
}

VAOLayout CreateVertexLayout(unsigned int sizes[], unsigned int bufflen, unsigned int n)
{
VAOLayout layout;
layout.sizes = (unsigned int*)malloc(n * sizeof(unsigned int)); // allocating the size
for(int i = 0; i < n; i++)
    {
    layout.sizes[i] = sizes[i];
    }
layout.bufflen = bufflen;
layout.number = n;

return layout;
}

void InitialiseVertexLayout(VAOLayout layout)
{
unsigned int stride = 0;
for(int i = 0; i < layout.number; i++)
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