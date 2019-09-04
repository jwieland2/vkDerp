#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>

#include "DerpSurface.h"
#include "DerpPhysicalDevice.h"
#include "DerpDevice.h"

class DerpSwapChain
{
public:
    DerpSwapChain(GLFWwindow* window,
        std::unique_ptr<DerpSurface>& surface,
        std::unique_ptr<DerpPhysicalDevice>& physicalDevice,
        std::unique_ptr<DerpDevice>& device);

    ~DerpSwapChain();

	vk::Format findDepthFormat(std::unique_ptr<DerpPhysicalDevice>& physicalDevice);

    vk::SwapchainKHR handle;

    std::vector<vk::Image> images;
    vk::Format imageFormat;
	vk::Format depthFormat;
    vk::Extent2D extent;
    std::vector<vk::ImageView> imageViews;
};

