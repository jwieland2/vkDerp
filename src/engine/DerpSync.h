#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"

class DerpSync
{
public:
	DerpSync(std::unique_ptr<DerpDevice>& device);
	~DerpSync();

	vk::Semaphore imageAvailableSemaphore;
	vk::Semaphore renderFinishedSemaphore;

	std::vector<vk::Fence> inFlightFences;

	const static int maxFramesInFlight = 2;
};

