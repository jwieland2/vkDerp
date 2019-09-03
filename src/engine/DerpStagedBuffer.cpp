#include "DerpStagedBuffer.h"


DerpStagedBuffer::DerpStagedBuffer(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<Vertex> v, VmaAllocator& allocator)
{
	std::cout << "create stagedBuffer vector" << std::endl;

	vk::DeviceSize bufferSize = sizeof(Vertex) * v.size();
	num = v.size();
	usage = vk::BufferUsageFlagBits::eVertexBuffer;

	auto ci = vk::BufferCreateInfo().
		setUsage(vk::BufferUsageFlagBits::eTransferSrc).
		setSharingMode(vk::SharingMode::eExclusive).
		setSize(bufferSize);

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	stagingBufferAllocation = VK_NULL_HANDLE;
	VmaAllocationInfo stagingBufferAllocInfo = {};

	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&ci, &allocCreateInfo, (VkBuffer*)&stagingBuffer, &stagingBufferAllocation, &stagingBufferAllocInfo);

	//for (const auto i : v)
	//	std::cout << i.pos.x << "\t" << i.pos.y << "\t" << i.color.x << "\t" << i.color.y << "\t" << i.color.z << std::endl;

	std::memcpy(stagingBufferAllocInfo.pMappedData, v.data(), bufferSize);

	ci.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
	allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocCreateInfo.flags = 0;
	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&ci, &allocCreateInfo, (VkBuffer*)&buffer, &bufferAllocation, nullptr);

	// copy staging buffer to vertex buffer
	auto allocCI = vk::CommandBufferAllocateInfo().
		setCommandBufferCount(1).
		setCommandPool(commandPool->handle).
		setLevel(vk::CommandBufferLevel::ePrimary);
	vk::CommandBuffer cmd = device->handle.allocateCommandBuffers(allocCI)[0];

	cmd.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

	vk::BufferCopy vbCopyRegion = {};
	vbCopyRegion.srcOffset = 0;
	vbCopyRegion.dstOffset = 0;
	vbCopyRegion.size = ci.size;
	cmd.copyBuffer(stagingBuffer, buffer, vbCopyRegion);

	cmd.end();

	auto submitInfo = vk::SubmitInfo().
		setCommandBufferCount(1).
		setPCommandBuffers(&cmd);
	device->graphicsQueue.submit({ submitInfo }, {});
	device->graphicsQueue.waitIdle();

	vmaDestroyBuffer(allocator, stagingBuffer, stagingBufferAllocation);
}

DerpStagedBuffer::DerpStagedBuffer(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<uint16_t> data, VmaAllocator& allocator)
{
	std::cout << "create stagedBuffer int" << std::endl;

	vk::DeviceSize bufferSize = sizeof(uint32_t) * data.size();
	num = data.size();
	usage = vk::BufferUsageFlagBits::eIndexBuffer;

	auto ci = vk::BufferCreateInfo().
		setUsage(vk::BufferUsageFlagBits::eTransferSrc).
		setSharingMode(vk::SharingMode::eExclusive).
		setSize(bufferSize);

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	allocCreateInfo.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	stagingBufferAllocation = VK_NULL_HANDLE;
	VmaAllocationInfo stagingBufferAllocInfo = {};

	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&ci, &allocCreateInfo, (VkBuffer*)&stagingBuffer, &stagingBufferAllocation, &stagingBufferAllocInfo);
	
	//for (const auto i : data)
	//	std::cout << i << "\t";
	//std::cout << std::endl;

	std::memcpy(stagingBufferAllocInfo.pMappedData, data.data(), bufferSize);

	ci.usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer;
	allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocCreateInfo.flags = 0;
	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&ci, &allocCreateInfo, (VkBuffer*)&buffer, &bufferAllocation, nullptr);

	// copy staging buffer to index buffer
	auto allocCI = vk::CommandBufferAllocateInfo().
		setCommandBufferCount(1).
		setCommandPool(commandPool->handle).
		setLevel(vk::CommandBufferLevel::ePrimary);
	vk::CommandBuffer cmd = device->handle.allocateCommandBuffers(allocCI)[0];

	cmd.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

	vk::BufferCopy vbCopyRegion = {};
	vbCopyRegion.srcOffset = 0;
	vbCopyRegion.dstOffset = 0;
	vbCopyRegion.size = ci.size;
	cmd.copyBuffer(stagingBuffer, buffer, vbCopyRegion);

	cmd.end();

	auto submitInfo = vk::SubmitInfo().
		setCommandBufferCount(1).
		setPCommandBuffers(&cmd);
	device->graphicsQueue.submit({ submitInfo }, {});
	device->graphicsQueue.waitIdle();

	vmaDestroyBuffer(allocator, stagingBuffer, stagingBufferAllocation);
}

DerpStagedBuffer::~DerpStagedBuffer()
{
}
