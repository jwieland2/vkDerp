#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"
#include "DerpSwapChain.h"

class DerpDescriptorPool
{
public:
	DerpDescriptorPool(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain);
	~DerpDescriptorPool();

	vk::DescriptorPool handle;
};

