#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"
#include "DerpDevice.h"
#include "DerpCommandPool.h"
#include "DerpFramebuffers.h"


class DerpCommandBuffer
{
public:
	DerpCommandBuffer(
		std::unique_ptr<DerpDevice>& device,
		std::unique_ptr<DerpCommandPool>& commandPool,
		std::unique_ptr<DerpFramebuffers>& framebuffers);

	~DerpCommandBuffer();

	std::vector<vk::CommandBuffer> handles;
};


