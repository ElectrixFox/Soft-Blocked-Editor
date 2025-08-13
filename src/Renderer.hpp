#ifndef RENDERER_HPP
#define RENDERER_HPP
#pragma once
#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Rendering/SpriteShapes.h"
#include "Text.hpp"

struct RenderObject
    {
    unsigned int vao = 0, vbo = 0, ibo = 0, prog = 0, tex = 0;

    RenderObject() {};

    RenderObject(SpriteSheetInfo ssi, int flip = 0)
        {
        viBundle vbund = GetShapeVertices(SHAPE::SHAPE_SQUARE, ssi.nosp, ssi.spr);  // the bundle containing the vertices and count
        viBundle ibund = GetShapeIndices(SHAPE::SHAPE_SQUARE);  // the bundle containing the indices and count

        // creating the shader
        prog = CreateShader("res/blockvert.shader", "res/blockfrag.shader");    // creates the shader object
        SetUniformM4(prog, "projection", getProjection(1280, 720, 1));  // setting up the projection
        SetUniformM4(prog, "view", getM4ID());  // setting up the view

        // creating the texture
        tex = CreateTexture(ssi.spfp, 0, flip);
        SetUniform1i(prog, "intexture", 0); // set the texture to be used (the 0th active texture)

        vao = CreateVAO();  // creating the vao
        ibo = CreateIBO(ibund.vi, ibund.n); // creating the ibo
        BindIBO(ibo);  // binding the ibo to the vao
        vbo = CreateVBO(vbund.vi, vbund.n); // creating the vbo
        BindVBO(vbo);  // binding the vbo to the vao
        
        unsigned int ilay[1] = {3};
        VAOLayout layout = CreateVertexLayout(ilay, 5, 1);  // setting up the layout to receive
        AddToVertexLayout(layout, 2);  // adding the texture coords to the layout
        InitialiseVertexLayout(layout); // initialising the layout to be used
        };

    RenderObject(const char* text)
        {
        viBundle vbund = GetShapeVertices(SHAPE::SHAPE_SQUARE, 1, 1);   // the bundle containing the vertices and count
        viBundle ibund = GetShapeIndices(SHAPE::SHAPE_SQUARE);  // the bundle containing the indices and count

        tex = getChar(text[0]);

        // creating the shader
        prog = CreateShader("res/textvert.shader", "res/textfrag.shader");    // creates the shader object
        SetUniformM4(prog, "projection", getProjection(1280, 720, 1));  // setting up the projection
        SetUniformM4(prog, "view", getM4ID());  // setting up the view

        // creating the texture
        SetUniform1i(prog, "intexture", 0); // set the texture to be used (the 1st active texture)

        vao = CreateVAO();  // creating the vao
        ibo = CreateIBO(ibund.vi, ibund.n); // creating the ibo
        BindIBO(ibo);  // binding the ibo to the vao
        vbo = CreateVBO(vbund.vi, vbund.n); // creating the vbo
        BindVBO(vbo);  // binding the vbo to the vao

        unsigned int ilay[1] = {3};
        VAOLayout layout = CreateVertexLayout(ilay, 5, 1);  // setting up the layout to receive
        AddToVertexLayout(layout, 2);  // adding the texture coords to the layout
        InitialiseVertexLayout(layout); // initialising the layout to be used
        }
    };

struct RenderObjectNode
    {
    unsigned int vao = 0, vbo = 0, ibo = 0, prog = 0, tex = 0;
    RenderObjectNode* next = nullptr;

    RenderObjectNode& operator=(const RenderObject& ro)
        {
        vao = ro.vao;
        vbo = ro.vbo;
        ibo = ro.ibo;
        prog = ro.prog;
        tex = ro.tex;

        return *this;
        }

    RenderObject get()
        {
        RenderObject ro;
        ro.vao = vao;
        ro.vbo = vbo;
        ro.ibo = ibo;
        ro.prog = prog;
        ro.tex = tex;
        return ro;
        }
    };

void drawRenderObject(RenderObject ro, vec2 pos, vec2 scale, float angle);

#endif