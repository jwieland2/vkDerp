#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"
#include "DerpCommandPool.h"
#include "DerpBufferStaging.h"
#include "vk_mem_alloc.h"
#include "Vertex.h"

class DerpBufferLocal
{
public:
	DerpBufferLocal(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<Vertex> vertices, VmaAllocator& allocator);
	DerpBufferLocal(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<uint16_t> indices, VmaAllocator& allocator);
	~DerpBufferLocal();

	void createLocalBuffer(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator);

	std::unique_ptr<DerpBufferStaging> stagingBuffer;

	vk::Buffer buffer;
	VmaAllocation allocation;
	vk::BufferUsageFlagBits usage;
	uint32_t num = 0; // numvertices or numindices
};

