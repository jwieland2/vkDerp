#include "FpsMonitor.h"

#include <string>

FpsMonitor::FpsMonitor()
{
	lastFrame = std::chrono::high_resolution_clock::now();
	glfwLastFrame = glfwGetTime()*1000;
}


FpsMonitor::~FpsMonitor()
{
}

void FpsMonitor::update()
{
	currentFrame = std::chrono::high_resolution_clock::now();
	glfwCurrentFrame = glfwGetTime()*1000;

	glfwdt = glfwCurrentFrame - glfwLastFrame;
	dt = std::chrono::duration<float, std::milli>(currentFrame - lastFrame).count();

	loss += abs(16.666666 - dt);
	glfwloss += abs(16.666666 - glfwdt);

	glfwLastFrame = glfwCurrentFrame;
	lastFrame = currentFrame;
}

void FpsMonitor::updateWindow(GLFWwindow* window, double d)
{
	if (timeSinceUpdate < d)
		timeSinceUpdate += dt;
	else
	{
		glfwSetWindowTitle(window, ("chrono frametime: " + to_string_with_precision(dt, 2) + " err: " + to_string_with_precision(loss, 2) + 
			" | glfw frametime: " + to_string_with_precision(glfwdt, 2) + " err: " + to_string_with_precision(glfwloss, 2)).c_str());
		timeSinceUpdate = 0.0;
	}
}

void FpsMonitor::reset()
{
	timeSinceUpdate = 0.0;
}
