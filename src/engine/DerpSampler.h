#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "Vertex.h"
#include "DerpDevice.h"

class DerpSampler
{
public:
	DerpSampler(DerpSamplerInit& init, std::unique_ptr<DerpDevice>& device);
	~DerpSampler();

	vk::Sampler handle;
};

