#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <iostream>

#include "Vertex.h"
#include "vk_mem_alloc.h"

class DerpBufferStaging
{
public:
	DerpBufferStaging(vk::DeviceSize bufferSize, VmaAllocator& allocator);
	~DerpBufferStaging();

	void copy(std::vector<Vertex> v);
	void copy(std::vector<uint16_t> data);
	void copy(unsigned char*&  imageData);

	void destroyVma(VmaAllocator& allocator);

	vk::DeviceSize size;
	
	vk::Buffer buffer;

	VmaAllocationInfo bufferAllocInfo = {};
	VmaAllocation allocation;
};

