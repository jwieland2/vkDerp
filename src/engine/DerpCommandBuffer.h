#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Vertex.h"
#include "DerpDevice.h"
#include "DerpSwapChain.h"
#include "DerpRenderPass.h"
#include "DerpPipeline.h"
#include "DerpCommandPool.h"
#include "DerpBufferLocal.h"
#include "DerpFramebuffers.h"
//#include "DerpStagedBuffer.h"


class DerpCommandBuffer
{
public:
	DerpCommandBuffer(
		std::unique_ptr<DerpDevice>& device,
		std::unique_ptr<DerpCommandPool>& commandPool,
		std::unique_ptr<DerpFramebuffers>& framebuffers,
		mvp4 &m);

	~DerpCommandBuffer();

	std::vector<vk::CommandBuffer> handles;
};


