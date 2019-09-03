#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include "DerpInstance.h"

class DerpSurface
{
public:
    DerpSurface(std::unique_ptr<DerpInstance>& instance, GLFWwindow* window);
    ~DerpSurface();

	VkSurfaceKHR vksurface;
	vk::UniqueSurfaceKHR handle;
	//vk::Instance& instanceRef;
};

