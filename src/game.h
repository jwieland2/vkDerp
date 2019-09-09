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



	std::vector<Vertex> vertices = {
		// positions          // normals           // texture coords

		{{-0.5f, -0.5f, -0.5f},{0.0f,  0.0f, -1.0f},{0.0f,  0.0f}},
		{{ 0.5f, -0.5f, -0.5f},{0.0f,  0.0f, -1.0f},{1.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f},{0.0f,  0.0f, -1.0f},{1.0f,  1.0f}},
		{{ 0.5f,  0.5f, -0.5f},{0.0f,  0.0f, -1.0f},{1.0f,  1.0f}},
		{{-0.5f,  0.5f, -0.5f},{0.0f,  0.0f, -1.0f},{0.0f,  1.0f}},
		{{-0.5f, -0.5f, -0.5f},{0.0f,  0.0f, -1.0f},{0.0f,  0.0f}},

		{{-0.5f, -0.5f,  0.5f},{0.0f,  0.0f,  1.0f},{0.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f},{0.0f,  0.0f,  1.0f},{1.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f},{0.0f,  0.0f,  1.0f},{1.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f},{0.0f,  0.0f,  1.0f},{1.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f},{0.0f,  0.0f,  1.0f},{0.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f},{0.0f,  0.0f,  1.0f},{0.0f,  0.0f}},

		{{-0.5f,  0.5f,  0.5f},{1.0f,  0.0f,  0.0f},{1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},{1.0f,  0.0f,  0.0f},{1.0f,  1.0f}},
		{{-0.5f, -0.5f, -0.5f},{1.0f,  0.0f,  0.0f},{0.0f,  1.0f}},
		{{-0.5f, -0.5f, -0.5f},{1.0f,  0.0f,  0.0f},{0.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f},{1.0f,  0.0f,  0.0f},{0.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},{1.0f,  0.0f,  0.0f},{1.0f,  0.0f}},

		{{ 0.5f,  0.5f,  0.5f},{1.0f,  0.0f,  0.0f},{1.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f},{1.0f,  0.0f,  0.0f},{1.0f,  1.0f}},
		{{ 0.5f, -0.5f, -0.5f},{1.0f,  0.0f,  0.0f},{0.0f,  1.0f}},
		{{ 0.5f, -0.5f, -0.5f},{1.0f,  0.0f,  0.0f},{0.0f,  1.0f}},
		{{ 0.5f, -0.5f,  0.5f},{1.0f,  0.0f,  0.0f},{0.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f},{1.0f,  0.0f,  0.0f},{1.0f,  0.0f}},

		{{-0.5f, -0.5f, -0.5f},{0.0f, -1.0f,  0.0f},{0.0f,  1.0f}},
		{{ 0.5f, -0.5f, -0.5f},{0.0f, -1.0f,  0.0f},{1.0f,  1.0f}},
		{{ 0.5f, -0.5f,  0.5f},{0.0f, -1.0f,  0.0f},{1.0f,  0.0f}},
		{{ 0.5f, -0.5f,  0.5f},{0.0f, -1.0f,  0.0f},{1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},{0.0f, -1.0f,  0.0f},{0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},{0.0f, -1.0f,  0.0f},{0.0f,  1.0f}},

		{{-0.5f,  0.5f, -0.5f},{0.0f,  1.0f,  0.0f},{0.0f,  1.0f}},
		{{ 0.5f,  0.5f, -0.5f},{0.0f,  1.0f,  0.0f},{1.0f,  1.0f}},
		{{ 0.5f,  0.5f,  0.5f},{0.0f,  1.0f,  0.0f},{1.0f,  0.0f}},
		{{ 0.5f,  0.5f,  0.5f},{0.0f,  1.0f,  0.0f},{1.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},{0.0f,  1.0f,  0.0f},{0.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},{0.0f,  1.0f,  0.0f},{0.0f,  1.0f}}
	};

	GameObjectVertex* cube;

	std::vector<Vertex> planeVertices = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		{{ 5.0f, -0.5f,  5.0f},{0.0f,  1.0f,  0.0f},{  2.0f, 0.0f,}},
		{{-5.0f, -0.5f,  5.0f},{0.0f,  1.0f,  0.0f},{  0.0f, 0.0f,}},
		{{-5.0f, -0.5f, -5.0f},{0.0f,  1.0f,  0.0f},{  0.0f, 2.0f,}},
		{{ 5.0f, -0.5f,  5.0f},{0.0f,  1.0f,  0.0f},{  2.0f, 0.0f,}},
		{{-5.0f, -0.5f, -5.0f},{0.0f,  1.0f,  0.0f},{  0.0f, 2.0f,}},
		{{ 5.0f, -0.5f, -5.0f},{0.0f,  1.0f,  0.0f},{  2.0f, 2.0f }}
	};

	GameObjectVertex* floor;

private:


    void mainLoop();
};