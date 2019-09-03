#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpPhysicalDevice.h"
#include "DerpDevice.h"

class DerpCommandPool
{
public:
    DerpCommandPool(std::unique_ptr<DerpPhysicalDevice>& physicalDevice, std::unique_ptr<DerpDevice>& device);
    ~DerpCommandPool();

	vk::CommandPool handle;
};

