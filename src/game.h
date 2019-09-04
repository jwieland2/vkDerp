#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#include "engine/DerpRenderer.h"

class Game
{
public:
    void run();

private:
	DerpRenderer *renderer;

    void mainLoop();
};