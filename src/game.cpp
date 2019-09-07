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

	vertexObjects.push_back(new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, vertices));
	vertexObjects.push_back(new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, planeVertices));

	
    mainLoop();
}

void Game::mainLoop()
{


    while (!glfwWindowShouldClose(renderer->window))
    {
        glfwPollEvents();
		input->process(camera);
		
		for (auto it : vertexObjects)
			it->draw(*renderer);

        renderer->drawFrame(camera);
		camera->updateCameraVectors();
    }

    renderer->device->handle.waitIdle();
	delete renderer;
}

