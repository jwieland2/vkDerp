#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpSwapChain.h"

class DerpRenderPass
{
public:
    DerpRenderPass(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain);
    ~DerpRenderPass();

	vk::RenderPass handle;
};

