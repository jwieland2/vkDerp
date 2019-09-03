#pragma once

#include <optional>
#include <fstream>
#include <array>
#include <iomanip>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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