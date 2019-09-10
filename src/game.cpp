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

	cube = new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, vertices);
	floor = new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, planeVertices);

	vertexObjects.push_back(cube);
	vertexObjects.push_back(floor);

	std::vector<Vertex> data;

	for (auto it : vertexObjects)
	{
		data.insert(data.end(), it->vertices_.begin(), it->vertices_.end());
	}

	renderer->vertexBuffer.reset();
	renderer->vertexBuffer = std::make_unique<DerpBufferLocal>(renderer->device, renderer->commandPool, data, renderer->allocator);

    mainLoop();
}

void Game::mainLoop()
{


    while (!glfwWindowShouldClose(renderer->window))
    {
        glfwPollEvents();
		input->process(camera, renderer->fpsMonitor.dt);

		renderer->beginDraw(camera);

		for (int i = 0; i < vertexObjects.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			if (i)
				model *= glm::yawPitchRoll((float)glm::radians(glfwGetTime()*50), 0.0f, 0.0f);
			else
				model *= glm::yawPitchRoll((float)glm::radians(glfwGetTime() * -50), 0.0f, 0.0f);

			renderer->drawObject(model, vertexObjects[i]->objVertexBuffer.get());
		}

        renderer->endDraw();
		camera->updateCameraVectors();
    }

    renderer->device->handle.waitIdle();
	for (auto o : vertexObjects)
		delete o;
	delete renderer;
}

