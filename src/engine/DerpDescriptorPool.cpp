#include "DerpDescriptorPool.h"



DerpDescriptorPool::DerpDescriptorPool(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain)
{
	std::cout << "create descriptorPool" << std::endl;

	std::array<vk::DescriptorPoolSize, 3> poolSizes = {};
	poolSizes[0].setType(vk::DescriptorType::eUniformBuffer);
	poolSizes[0].setDescriptorCount(static_cast<uint32_t>(swapChain->images.size()));
	poolSizes[1].setType(vk::DescriptorType::eSampledImage);
	poolSizes[1].setDescriptorCount(8);
	poolSizes[2].setType(vk::DescriptorType::eSampler);
	poolSizes[2].setDescriptorCount(1);

	vk::DescriptorPoolCreateInfo descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo().
		setPoolSizeCount(static_cast<uint32_t>(poolSizes.size())).
		setPPoolSizes(poolSizes.data()).
		setMaxSets(static_cast<uint32_t>(swapChain->images.size()));

	handle = device->handle.createDescriptorPool(descriptorPoolCreateInfo);
}


DerpDescriptorPool::~DerpDescriptorPool()
{
}
