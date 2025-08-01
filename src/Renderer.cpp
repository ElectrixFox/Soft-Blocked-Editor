#include "Renderer.hpp"

void drawRenderObject(RenderObject ro, vec2 pos, vec2 scale, float angle)
{
m4 model = GetModelMatrix(pos, scale, angle);

SetUniformM4(ro.prog, "model", model); // setting the model matrix

if(ro.tex != 0)
    BindTexture(ro.tex);

BindShader(ro.prog);
BindVAO(ro.vao);

glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}