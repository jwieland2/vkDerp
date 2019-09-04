#include "DerpBufferLocal.h"

#include <iostream>

DerpBufferLocal::DerpBufferLocal(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<Vertex> vertices, VmaAllocator& allocator)
{
	std::cout << "create local buffer: vertex" << std::endl;
	usage = vk::BufferUsageFlagBits::eVertexBuffer;
	num = vertices.size();
	stagingBuffer = std::make_unique<DerpBufferStaging>(sizeof(Vertex) * num, allocator);
	stagingBuffer->copy(vertices);

	createLocalBuffer(device, commandPool, allocator);

}

DerpBufferLocal::DerpBufferLocal(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, std::vector<uint16_t> indices, VmaAllocator& allocator)
{
	std::cout << "create local buffer: index" << std::endl;
	num = indices.size();
	usage = vk::BufferUsageFlagBits::eIndexBuffer;
	stagingBuffer = std::make_unique<DerpBufferStaging>(sizeof(uint16_t) * num, allocator);
	stagingBuffer->copy(indices);

	createLocalBuffer(device, commandPool, allocator);

}


DerpBufferLocal::~DerpBufferLocal()
{
}

void DerpBufferLocal::createLocalBuffer(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator)
{
	vk::BufferCreateInfo bufferCreateInfo = vk::BufferCreateInfo().
	setUsage(vk::BufferUsageFlagBits::eTransferDst | usage).
	setSharingMode(vk::SharingMode::eExclusive).
	setSize(stagingBuffer->size);

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	allocCreateInfo.flags = 0;

	vmaCreateBuffer(allocator, (VkBufferCreateInfo*)&bufferCreateInfo, &allocCreateInfo, (VkBuffer*)&buffer, &allocation, nullptr);

	// copy staging buffer to local buffer
	auto allocInfo = vk::CommandBufferAllocateInfo().
	setCommandBufferCount(1).
	setCommandPool(commandPool->handle).
	setLevel(vk::CommandBufferLevel::ePrimary);
	vk::CommandBuffer cmd = device->handle.allocateCommandBuffers(allocInfo)[0];

	cmd.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

	vk::BufferCopy vbCopyRegion = {};
	vbCopyRegion.srcOffset = 0;
	vbCopyRegion.dstOffset = 0;
	vbCopyRegion.size = bufferCreateInfo.size;
	cmd.copyBuffer(stagingBuffer->buffer, buffer, vbCopyRegion);

	cmd.end();

	auto submitInfo = vk::SubmitInfo().
	setCommandBufferCount(1).
	setPCommandBuffers(&cmd);
	device->graphicsQueue.submit({ submitInfo }, {});
	device->graphicsQueue.waitIdle();

	stagingBuffer->destroyVma(allocator);
	stagingBuffer.reset();
}