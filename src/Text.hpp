#ifndef TEXT_HPP
#define TEXT_HPP
#pragma once
#include "Utils.h"
#include "MathsOperations.h"
#include "Rendering/Buffer.h"
#include "Rendering/Texture.h"
#include "Rendering/Shader.h"
#include "Rendering/SpriteShapes.h"
#include "FileHandling.h"
#include <iostream>
#include <map>

void initfont();

unsigned int getChar(char ch);

#endif