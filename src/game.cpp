#include "game.h"

#include "data.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::run()
{
	
	renderer = new DerpRenderer;
	//terrain = new Terrain(renderer);
	camera = new Camera(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glfwSetWindowUserPointer(renderer->window, this);
	input = new Input(renderer);

	//cube = new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, cubeVertices);
	floor = new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, planeVertices);
	//square = new GameObjectVertex(renderer->device, renderer->commandPool, renderer->allocator, squareVertices, squareIndices);

	//vertexObjects.push_back(cube);
	vertexObjects.push_back(floor);
	//vertexObjects.push_back(square);

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
			glm::mat4 model = vertexObjects[i]->model;
			if (i)
				model *= glm::yawPitchRoll((float)glm::radians(glfwGetTime()*50), 0.0f, 0.0f);
			else
				model *= glm::yawPitchRoll((float)glm::radians(glfwGetTime() * -50), 0.0f, 0.0f);

			if (vertexObjects[i]->numIndices)
				renderer->drawObject(model, vertexObjects[i]->objVertexBuffer.get(), vertexObjects[i]->objIndexBuffer.get());
			else
				renderer->drawObject(model, vertexObjects[i]->objVertexBuffer.get());
		}

		//renderer->drawObject(glm::mat4(1.0f), terrain->vertexBuffer.get(), terrain->indexBuffer.get());

        renderer->endDraw();
		camera->updateCameraVectors();
    }

    renderer->device->handle.waitIdle();
	for (auto o : vertexObjects)
		delete o;
	delete terrain;
	delete renderer;
}

//void Game::updateUniformBuffers()
//{
//	// Tessellation
//
//	uboTess.projection = camera.matrices.perspective;
//	uboTess.modelview = camera.matrices.view * glm::mat4(1.0f);
//	uboTess.lightPos.y = -0.5f - uboTess.displacementFactor; // todo: Not uesed yet
//	uboTess.viewportDim = glm::vec2((float)width, (float)height);
//
//	frustum.update(uboTess.projection * uboTess.modelview);
//	memcpy(uboTess.frustumPlanes, frustum.planes.data(), sizeof(glm::vec4) * 6);
//
//	float savedFactor = uboTess.tessellationFactor;
//	if (!tessellation)
//	{
//		// Setting this to zero sets all tessellation factors to 1.0 in the shader
//		uboTess.tessellationFactor = 0.0f;
//	}
//
//	memcpy(uniformBuffers.terrainTessellation.mapped, &uboTess, sizeof(uboTess));
//
//	if (!tessellation)
//	{
//		uboTess.tessellationFactor = savedFactor;
//	}
//
//	// Skysphere vertex shader
//	uboVS.mvp = camera.matrices.perspective * glm::mat4(glm::mat3(camera.matrices.view));
//	memcpy(uniformBuffers.skysphereVertex.mapped, &uboVS, sizeof(uboVS));
//}