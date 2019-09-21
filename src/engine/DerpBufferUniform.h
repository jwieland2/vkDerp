#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"
#include "DerpSwapChain.h"

#include <glm/glm.hpp>

#include "vk_mem_alloc.h"
#include "Vertex.h"



class DerpBufferUniform
{
public:
	DerpBufferUniform(std::unique_ptr<DerpDevice>& device, VmaAllocator& allocator);
	~DerpBufferUniform();

	std::vector<vk::Buffer> uniformBuffers;
	VmaAllocation bufferAllocation;

	VmaAllocationInfo uniformBufferAllocInfo = {};
	mvp4 *data;
};

