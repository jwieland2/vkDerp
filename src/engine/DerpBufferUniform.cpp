#include "DerpBufferUniform.h"



DerpBufferUniform::DerpBufferUniform(std::unique_ptr<DerpDevice> & device, std::unique_ptr<DerpSwapChain>& swapChain, VmaAllocator& allocator)
{
	std::cout << "create uniform buffer" << std::endl;
	vk::DeviceSize bufferSize = sizeof(color);

    uniformBuffers.resize(1);

	auto ci = vk::BufferCreateInfo().
		setUsage(vk::BufferUsageFlagBits::eUniformBuffer).
		setSharingMode(vk::SharingMode::eExclusive).
		setSize(bufferSize);

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	bufferAllocation = VK_NULL_HANDLE;


	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&ci, &allocCreateInfo, (VkBuffer*)&(uniformBuffers[0]), &bufferAllocation, &uniformBufferAllocInfo);

	data = (color*)uniformBufferAllocInfo.pMappedData;
}


DerpBufferUniform::~DerpBufferUniform()
{
}
