#include "DerpDevice.h"

#include <set>


DerpDevice::DerpDevice(std::unique_ptr<DerpPhysicalDevice>& physicalDevice)
{
    std::cout << "create device" << std::endl;
    float queuePriority = 1.0f;
    vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures().
		setSamplerAnisotropy(VK_TRUE);

    std::vector<vk::DeviceQueueCreateInfo> dqciV;
    std::set<uint32_t> uniqueQueueFamilies = { physicalDevice->queueFamilyIndices.graphicsFamily.value(), physicalDevice->queueFamilyIndices.presentFamily.value() };

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        vk::DeviceQueueCreateInfo dqci({}, queueFamily, 1, &queuePriority);
        dqciV.push_back(dqci);
    }

    vk::DeviceCreateInfo dci({},
        static_cast<uint32_t>(dqciV.size()), dqciV.data(),
        0, nullptr,
        static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
        &deviceFeatures);

    if (enableValidationLayers)
    {
        dci.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        dci.ppEnabledLayerNames = validationLayers.data();
    }

    handle = physicalDevice->handle.createDevice(dci);

    graphicsQueue = handle.getQueue(physicalDevice->queueFamilyIndices.graphicsFamily.value(), 0);
    presentQueue = handle.getQueue(physicalDevice->queueFamilyIndices.presentFamily.value(), 0);
}


DerpDevice::~DerpDevice()
{
}
