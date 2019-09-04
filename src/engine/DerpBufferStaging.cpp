#include "DerpBufferStaging.h"



DerpBufferStaging::DerpBufferStaging(vk::DeviceSize bufferSize, VmaAllocator& allocator) : size(bufferSize)
{
	std::cout << "create stagingBuffer" << std::endl;

	auto ci = vk::BufferCreateInfo().
		setUsage(vk::BufferUsageFlagBits::eTransferSrc).
		setSharingMode(vk::SharingMode::eExclusive).
		setSize(bufferSize);

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	allocation = VK_NULL_HANDLE;

	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&ci, &allocCreateInfo, (VkBuffer*)&buffer, &allocation, &bufferAllocInfo);
}


DerpBufferStaging::~DerpBufferStaging()
{
}

void DerpBufferStaging::copy(std::vector<Vertex> data)
{
	std::memcpy(bufferAllocInfo.pMappedData, data.data(), size);
}

void DerpBufferStaging::copy(std::vector<uint16_t> data)
{
	std::memcpy(bufferAllocInfo.pMappedData, data.data(), size);
}

void DerpBufferStaging::copy(unsigned char*& data)
{
	std::memcpy(bufferAllocInfo.pMappedData, data, size);
}

void DerpBufferStaging::destroyVma(VmaAllocator& allocator)
{
	vmaDestroyBuffer(allocator, buffer, allocation);
}
