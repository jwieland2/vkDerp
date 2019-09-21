#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <string>

#include "DerpCommandPool.h"
#include "DerpBufferStaging.h"
#include "DerpSwapChain.h"

#include "vk_mem_alloc.h"

class DerpImage
{
public:
	DerpImage();
	~DerpImage();

	void createTexture(
		DerpImageInit& init,
		std::unique_ptr<DerpDevice>& device,
		std::unique_ptr<DerpCommandPool>& commandPool,
		VmaAllocator& allocator);

	void createDepthBuffer(
		std::unique_ptr<DerpPhysicalDevice>& physicalDevice,
		std::unique_ptr<DerpDevice>& device,
		std::unique_ptr<DerpSwapChain>& swapChain,
		VmaAllocator& allocator);



	vk::Image handle;
	vk::Format format;
	vk::ImageView view;
	VmaAllocation allocation;
};