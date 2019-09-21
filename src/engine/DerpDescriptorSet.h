#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"
#include "DerpSwapChain.h"
#include "DerpImage.h"
#include "DerpSampler.h"
#include "DerpDescriptorSetLayout.h"
#include "DerpDescriptorPool.h"
#include "DerpBufferUniform.h"

#include "Vertex.h"

class DerpDescriptorSet
{
public:
	DerpDescriptorSet(
		DerpDescriptorInit& init,
		std::unique_ptr<DerpDevice>& device,
		std::unique_ptr<DerpSwapChain>& swapChain,
		std::unique_ptr<DerpDescriptorSetLayout>& layout,
		std::unique_ptr<DerpDescriptorPool>& descriptorPool,
		std::unique_ptr<DerpBufferUniform>& derpBufferUniform);
	~DerpDescriptorSet();

	vk::DescriptorSet handle;

	vk::DescriptorBufferInfo uniformDescriptor;
	vk::DescriptorImageInfo imageDescriptor;
	vk::DescriptorImageInfo heightDescriptor;
	vk::DescriptorImageInfo samplerDescriptor;
};

