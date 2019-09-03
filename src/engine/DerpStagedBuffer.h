#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "DerpCommandPool.h"
#include "DerpSwapChain.h"

#include "Vertex.h"
#include "vk_mem_alloc.h"

class DerpStagedBuffer
{
public:
	DerpStagedBuffer(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<Vertex> v, VmaAllocator& allocator);
	DerpStagedBuffer(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<uint16_t> data, VmaAllocator& allocator);
	~DerpStagedBuffer();

	vk::Buffer stagingBuffer;
	VmaAllocation stagingBufferAllocation;
	vk::Buffer buffer;
	VmaAllocation bufferAllocation;
	uint32_t num = 0; // numvertices or numindices
	vk::BufferUsageFlagBits usage;
};

