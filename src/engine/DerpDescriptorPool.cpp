#include "DerpDescriptorPool.h"



DerpDescriptorPool::DerpDescriptorPool(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain)
{
	std::cout << "create descriptorPool" << std::endl;

	std::array<vk::DescriptorPoolSize, 2> poolSizes = {};
	poolSizes[0].setType(vk::DescriptorType::eUniformBuffer);
	poolSizes[0].setDescriptorCount(static_cast<uint32_t>(swapChain->images.size()));
	poolSizes[1].setType(vk::DescriptorType::eCombinedImageSampler);
	poolSizes[1].setDescriptorCount(static_cast<uint32_t>(swapChain->images.size()));

	vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo().
		setPoolSizeCount(static_cast<uint32_t>(poolSizes.size())).
		setPPoolSizes(poolSizes.data()).
		setMaxSets(static_cast<uint32_t>(swapChain->images.size()));

	handle = device->handle.createDescriptorPool(descriptorPoolCreateInfo);
}


DerpDescriptorPool::~DerpDescriptorPool()
{
}
