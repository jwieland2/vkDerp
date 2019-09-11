#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

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

	std::vector<GameObjectVertex*> vertexObjects;

	GameObjectVertex* cube;
	GameObjectVertex* floor;

private:


    void mainLoop();
};