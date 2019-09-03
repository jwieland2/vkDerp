#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"

class DerpDescriptorSetLayout
{
public:
	DerpDescriptorSetLayout(std::unique_ptr<DerpDevice>& device);
	~DerpDescriptorSetLayout();

	vk::DescriptorSetLayout handle;
};

