#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"

class DerpSampler
{
public:
	DerpSampler(std::unique_ptr<DerpDevice>& device);
	~DerpSampler();

	vk::Sampler handle;
};

