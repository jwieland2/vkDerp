#pragma once
#include <optional>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include "DerpInstance.h"
#include "DerpSurface.h"

class DerpPhysicalDevice
{
public:
    DerpPhysicalDevice(std::unique_ptr<DerpInstance>& instance, std::unique_ptr<DerpSurface>& surface);
    ~DerpPhysicalDevice();

    struct SwapChainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;

        void query(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface)
        {
            capabilities = device.getSurfaceCapabilitiesKHR(surface.get());
            formats = device.getSurfaceFormatsKHR(surface.get());
            presentModes = device.getSurfacePresentModesKHR(surface.get());
        }
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }

        void query(vk::PhysicalDevice device, vk::UniqueSurfaceKHR& surface)
        {
            std::vector<vk::QueueFamilyProperties> qfp = device.getQueueFamilyProperties();
            int i = 0;

            for (const auto& queueFamily : qfp)
            {
                if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, surface.get()))
                    presentFamily = i;

                if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
                    graphicsFamily = i;

                if (isComplete())
                    break;

                i++;
            }
        }
    };

    QueueFamilyIndices queueFamilyIndices;
    SwapChainSupportDetails swapChainSupport;
    vk::PhysicalDevice handle;
    bool checkDeviceExtensionSupport(vk::PhysicalDevice device);
};

