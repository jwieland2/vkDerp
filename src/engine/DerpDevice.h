#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include "DerpPhysicalDevice.h"

class DerpDevice
{
public:
    DerpDevice(std::unique_ptr<DerpPhysicalDevice>& physicalDevice);
    ~DerpDevice();

    vk::Device handle;
    vk::Queue graphicsQueue;
    vk::Queue presentQueue;
};

