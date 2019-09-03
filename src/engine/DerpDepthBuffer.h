#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

class DerpDepthBuffer
{
public:
	DerpDepthBuffer();
	~DerpDepthBuffer();

	vk::Image image;
	vk::ImageView imageView;
};

