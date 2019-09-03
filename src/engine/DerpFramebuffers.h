#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "DerpDevice.h"
#include "DerpSwapChain.h"
#include "DerpImage.h"
#include "DerpRenderPass.h"

class DerpFramebuffers
{
public:
    DerpFramebuffers(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain, std::unique_ptr<DerpImage>& depthBuffer, std::unique_ptr<DerpRenderPass>& renderPass);
    ~DerpFramebuffers();

	std::vector<vk::Framebuffer> handles;
};

