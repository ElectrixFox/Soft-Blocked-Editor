#ifndef BLOCKS_H
#define BLOCKS_H
#pragma once
#include "MathsOperations.h"

#include "Rendering/SpriteShapes.h"
#include "Rendering/Buffer.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

#include <iostream>
#include <vector>
#include <functional>

class Entity
    {
    unsigned int id;
    };

struct RenderInfo
    {
    unsigned int vao;
    unsigned int vbo;
    unsigned int ibo;
    unsigned int shader;
    unsigned int texture;
    };

struct Block
    {
    // transformation stuff
    vec2 pos;   // the position
    vec2 scale; // the scale
    float rotation; // rotation (in radians)

    // rendering stuff
    SpriteSheetInfo ssi;    // the sprite info
    int fliptex = 0;
    RenderInfo ri;
    
    // important procedures
    std::function<void (Block& blk)>update;

    void draw(Block blk)
        {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        };
    
    Block(vec2 in_pos, vec2 in_scale, float in_rot, SpriteSheetInfo in_ssi, int flip = 0)
            : pos(in_pos), scale(in_scale), rotation(in_rot), ssi(in_ssi), fliptex(flip)
        {
        unsigned int vao, vbo, ibo, prog, tex = 0;
        unsigned long int shape = 0;

        GeneralInitialise(&shape, ssi.nosp, ssi.spr, SHAPE::SHAPE_SQUARE);


        viBundle vbund = GetShapeVertices(shape);   // the bundle containing the vertices and count
        viBundle ibund = GetShapeIndices(shape);    // the bundle containing the indices and count

        // creating the shader
        prog = CreateShader("res/blockvert.shader", "res/blockfrag.shader");    // creates the shader object
        SetUniformM4(prog, "projection", getProjection(1020, 960, 1));  // setting up the projection

        // creating the texture
        tex = CreateTexture(ssi.spfp, flip);
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

        ri = (RenderInfo){vao, vbo, ibo, prog, tex};
        };
    };

struct BlockManager
    {
    std::vector<Block> blocks;

    void updateBlocks()
        {
        for (Block block : blocks)
            block.update(block);
        }
    
    void drawBlocks()
        {
        for (Block block : blocks)
            block.draw(block);
        }
    };

#endif