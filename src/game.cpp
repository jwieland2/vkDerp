#include "game.h"

#include <stdexcept>
#include <iostream>
//#include <set>
#include <chrono>

void Game::run()
{
	renderer = new DerpRenderer;
    mainLoop();
}

void Game::mainLoop()
{
    while (!glfwWindowShouldClose(renderer->window))
    {
        glfwPollEvents();
        renderer->drawFrame();
    }

    renderer->device->handle.waitIdle();
}

