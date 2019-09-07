#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#include "engine/DerpRenderer.h"
#include "Input.h"
#include "Camera.h"
#include "GameObjectVertex.h"

class Game
{
public:
	Game();
	~Game();

    void run();

	

	DerpRenderer *renderer;
	Input *input;
	Camera *camera;

private:


    void mainLoop();
};