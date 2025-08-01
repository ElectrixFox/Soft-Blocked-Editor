#ifndef ENTITY_HPP
#define ENTITY_HPP
#pragma once
#include "Renderer.hpp"
#include <functional>

class Entity
    {
    public:
        Entity();

        void draw();
        std::function<void (Entity&)> update();
    };

#endif