#include "game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::run()
{
	
	renderer = new DerpRenderer;
	camera = new Camera(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glfwSetWindowUserPointer(renderer->window, this);
	input = new Input(renderer);

	
    mainLoop();
}

void Game::mainLoop()
{

	

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

	GameObjectVertex cube(renderer->device, renderer->commandPool, renderer->allocator, vertices);

	std::vector<Vertex> planeVertices = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		{{ 5.0f, -0.5f,  5.0f},{0.0f,  1.0f,  0.0f},{  2.0f, 0.0f,}},
		{{-5.0f, -0.5f,  5.0f},{0.0f,  1.0f,  0.0f},{  0.0f, 0.0f,}},
		{{-5.0f, -0.5f, -5.0f},{0.0f,  1.0f,  0.0f},{  0.0f, 2.0f,}},
		{{ 5.0f, -0.5f,  5.0f},{0.0f,  1.0f,  0.0f},{  2.0f, 0.0f,}},
		{{-5.0f, -0.5f, -5.0f},{0.0f,  1.0f,  0.0f},{  0.0f, 2.0f,}},
		{{ 5.0f, -0.5f, -5.0f},{0.0f,  1.0f,  0.0f},{  2.0f, 2.0f }}
	};

	GameObjectVertex floor(renderer->device, renderer->commandPool, renderer->allocator, planeVertices);

    while (!glfwWindowShouldClose(renderer->window))
    {
        glfwPollEvents();
		floor.draw(*renderer);
		cube.draw(*renderer);
        renderer->drawFrame(camera);
		camera->updateCameraVectors();
    }

    renderer->device->handle.waitIdle();
	delete renderer;
}

