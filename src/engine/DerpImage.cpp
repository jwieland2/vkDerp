#include "DerpImage.h"

#include <stb/stb_image.h>

DerpImage::DerpImage()
{
}


DerpImage::~DerpImage()
{
}

void DerpImage::createTexture(std::string file, std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator)
{
	std::cout << "create texture" << std::endl;

	int texWidth, texHeight, texChannels;

	// read file

	unsigned char* pixels = stbi_load(file.c_str(), &texWidth, &texHeight, &texChannels, 0);
	vk::DeviceSize imageSize = texWidth * texHeight * sizeof(unsigned char);
	imageSize = texWidth * texHeight * sizeof(uint8_t);

	std::cout << "\t" << texWidth << "x" << texHeight << ", " << texChannels << " channels, imageSize=" << imageSize << std::endl;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	// copy to staging

	std::unique_ptr<DerpBufferStaging> stagingBuffer = std::make_unique<DerpBufferStaging>(imageSize, allocator);

	stagingBuffer->copy(pixels);

	stbi_image_free(pixels);

	vk::ImageCreateInfo imageCreateInfo = vk::ImageCreateInfo().
		setImageType(vk::ImageType::e2D).
		setExtent(vk::Extent3D(static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1)).
		setMipLevels(1).
		setArrayLayers(1).
		//setFormat(vk::Format::eR8G8B8A8Unorm).
		setFormat(vk::Format::eR8Unorm).
		setTiling(vk::ImageTiling::eOptimal).
		setInitialLayout(vk::ImageLayout::eUndefined).
		setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled).
		setSharingMode(vk::SharingMode::eExclusive).
		setSamples(vk::SampleCountFlagBits::e1).
		setFlags({});

	VmaAllocationCreateInfo imageAllocCreateInfo = {};
	imageAllocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	vmaCreateImage(allocator, (VkImageCreateInfo*)&imageCreateInfo, &imageAllocCreateInfo, (VkImage*)&this->handle, &this->allocation, nullptr);


	// copy to gpu

	auto allocCI = vk::CommandBufferAllocateInfo().
		setCommandBufferCount(1).
		setCommandPool(commandPool->handle).
		setLevel(vk::CommandBufferLevel::ePrimary);
	vk::CommandBuffer cmd = device->handle.allocateCommandBuffers(allocCI)[0];

	cmd.begin({ vk::CommandBufferUsageFlagBits::eOneTimeSubmit });

	vk::ImageSubresourceRange imageSubresourceRange = vk::ImageSubresourceRange().
		setAspectMask(vk::ImageAspectFlagBits::eColor).
		setBaseMipLevel(0).
		setLevelCount(1).
		setBaseArrayLayer(0).
		setLayerCount(1);

	vk::ImageMemoryBarrier imageMemoryBarrier = vk::ImageMemoryBarrier().
		setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED).
		setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED).
		setSubresourceRange(imageSubresourceRange).
		setOldLayout(vk::ImageLayout::eUndefined).
		setNewLayout(vk::ImageLayout::eTransferDstOptimal).
		setImage(this->handle).
		setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

	cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, {}, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	vk::BufferImageCopy region = vk::BufferImageCopy().
		setImageSubresource(vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1)).
		setImageExtent(vk::Extent3D(texWidth, texHeight, 1));

	cmd.copyBufferToImage(stagingBuffer->buffer, this->handle, vk::ImageLayout::eTransferDstOptimal, 1, &region);

	imageMemoryBarrier.
		setOldLayout(vk::ImageLayout::eTransferDstOptimal).
		setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal).
		setImage(this->handle).
		setSrcAccessMask(vk::AccessFlagBits::eTransferWrite).
		setDstAccessMask(vk::AccessFlagBits::eShaderRead);

	cmd.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, {}, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

	cmd.end();

	vk::SubmitInfo submitInfo = vk::SubmitInfo().
		setCommandBufferCount(1).
		setPCommandBuffers(&cmd);
	device->graphicsQueue.submit(submitInfo, nullptr);
	device->graphicsQueue.waitIdle();
	device->handle.freeCommandBuffers(commandPool->handle, cmd);



	vmaDestroyBuffer(allocator, stagingBuffer->buffer, stagingBuffer->allocation);

	// image view

	vk::ImageSubresourceRange imageSubresourceRange2 = vk::ImageSubresourceRange().
		setAspectMask(vk::ImageAspectFlagBits::eColor).
		setBaseMipLevel(0).
		setLevelCount(1).
		setBaseArrayLayer(0).
		setLayerCount(1);

	vk::ImageViewCreateInfo imageViewCreateInfo = vk::ImageViewCreateInfo().
		setImage(this->handle).
		setViewType(vk::ImageViewType::e2D).
		//setFormat(vk::Format::eR8G8B8A8Unorm).
		setFormat(vk::Format::eR8Unorm).
		setSubresourceRange(imageSubresourceRange2);

	view = device->handle.createImageView(imageViewCreateInfo);
}

void DerpImage::createDepthBuffer(std::unique_ptr<DerpPhysicalDevice>& physicalDevice, std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain, VmaAllocator& allocator)
{
	std::cout << "create depthBuffer" << std::endl;
	format = swapChain->depthFormat;
	vk::FormatProperties formatProperties = physicalDevice->handle.getFormatProperties(format);

	auto ci = vk::ImageCreateInfo().
		setImageType(vk::ImageType::e2D).
		setFormat(format).
		setExtent(vk::Extent3D(swapChain->extent, 1)).
		setMipLevels(1).
		setArrayLayers(1).
		setSamples(vk::SampleCountFlagBits::e1).
		setTiling(vk::ImageTiling::eOptimal).
		setInitialLayout(vk::ImageLayout::eUndefined).
		setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment).
		setSharingMode(vk::SharingMode::eExclusive);

	VmaAllocationCreateInfo allocCreateInfo = {};
	allocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	vmaCreateImage(allocator, (VkImageCreateInfo*)&ci, &allocCreateInfo, (VkImage*)&handle, &allocation, nullptr);

	vk::ImageSubresourceRange imageSubresourceRange = vk::ImageSubresourceRange().
		setAspectMask(vk::ImageAspectFlagBits::eDepth).
		setBaseMipLevel(0).
		setLevelCount(1).
		setBaseArrayLayer(0).
		setLayerCount(1);

	vk::ImageViewCreateInfo imageViewCreateInfo = vk::ImageViewCreateInfo().
		setImage(handle).
		setViewType(vk::ImageViewType::e2D).
		setFormat(format).
		setSubresourceRange(imageSubresourceRange);

	view = device->handle.createImageView(imageViewCreateInfo);
}