#include "game.h"

void Game::run()
{
	
	renderer = new DerpRenderer;
	camera = new Camera(glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glfwSetWindowUserPointer(renderer->window, this);
	input = new Input(renderer);
    mainLoop();
}

void Game::mainLoop()
{
    while (!glfwWindowShouldClose(renderer->window))
    {
        glfwPollEvents();
        renderer->drawFrame(camera);
		camera->updateCameraVectors();
    }

    renderer->device->handle.waitIdle();
	delete renderer;
}

