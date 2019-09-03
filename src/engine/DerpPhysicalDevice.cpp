#include "DerpPhysicalDevice.h"

#include <set>



DerpPhysicalDevice::DerpPhysicalDevice(std::unique_ptr<DerpInstance>& instance, std::unique_ptr<DerpSurface>& surface)
{
    std::cout << "create physicalDevice" << std::endl;
    std::vector<vk::PhysicalDevice> physicalDevices;
    physicalDevices = instance->handle.enumeratePhysicalDevices();

    for (const auto& device : physicalDevices)
    {
        vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
        vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();
        bool extensionsSupported = checkDeviceExtensionSupport(device);
        swapChainSupport.query(device, surface->handle);
        bool swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        queueFamilyIndices.query(device, surface->handle);

		// print info
		std::cout << "\tDevice: " << deviceProperties.deviceName << std::endl;
		std::cout << "\tType: " << vk::to_string(deviceProperties.deviceType) << std::endl;
		std::cout << "\tpushConstants max bytes: " << deviceProperties.limits.maxPushConstantsSize << std::endl;


        // isDeviceSuitable
        if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu &&
            queueFamilyIndices.isComplete() &&
            extensionsSupported &&
            swapChainAdequate)
        {
            handle = device;
            break;
        }
    }

    if (!handle) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}


DerpPhysicalDevice::~DerpPhysicalDevice()
{
}

bool DerpPhysicalDevice::checkDeviceExtensionSupport(vk::PhysicalDevice device)
{
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}