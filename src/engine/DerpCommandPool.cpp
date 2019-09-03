#include "DerpCommandPool.h"



DerpCommandPool::DerpCommandPool(std::unique_ptr<DerpPhysicalDevice>& physicalDevice, std::unique_ptr<DerpDevice>& device)
{
	std::cout << "create commandPool" << std::endl;

    vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer, physicalDevice->queueFamilyIndices.graphicsFamily.value());
    handle = device->handle.createCommandPool(createInfo);
}


DerpCommandPool::~DerpCommandPool()
{
}
