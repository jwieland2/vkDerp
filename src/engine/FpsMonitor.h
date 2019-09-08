#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <chrono>

#include "Vertex.h"

class FpsMonitor
{
public:
	FpsMonitor();
	~FpsMonitor();

	void update();
	void updateWindow(GLFWwindow* window, double d);
	void reset();

	std::chrono::time_point<std::chrono::steady_clock> lastFrame, currentFrame;
	double glfwLastFrame, glfwCurrentFrame;
	double dt, timeSinceUpdate = 0.0;
	double glfwdt, glfwTimeSinceUpdate;
	double loss = 0.0;
	double glfwloss = 0.0;
};

