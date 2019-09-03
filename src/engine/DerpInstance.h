#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vulkan/vulkan.hpp>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class DerpInstance
{
public:
    DerpInstance();
    ~DerpInstance();
    bool checkValidationLayerSupport();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);
	void printInfo();

    vk::InstanceCreateInfo createInfo;
    vk::ApplicationInfo appInfo;
    vk::Instance handle;
    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    vk::DebugUtilsMessengerEXT debugMessenger;
    vk::DispatchLoaderDynamic dynamicLoader;
};