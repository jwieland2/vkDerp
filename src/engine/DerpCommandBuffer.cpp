#include "DerpCommandBuffer.h"



DerpCommandBuffer::DerpCommandBuffer(
	std::unique_ptr<DerpDevice>& device,
	std::unique_ptr<DerpSwapChain>& swapChain,
	std::unique_ptr<DerpRenderPass>& renderPass,
	std::unique_ptr<DerpPipeline>& pipeline,
	std::unique_ptr<DerpCommandPool>& commandPool,
	std::unique_ptr<DerpFramebuffers>& framebuffers,
	std::unique_ptr<DerpStagedBuffer>& vertexBuffer,
	std::unique_ptr<DerpStagedBuffer>& indexBuffer,
	mvp4 &m)
{
	std::cout << "create commandBuffers" << std::endl;

    handles.resize(framebuffers->handles.size());

    vk::CommandBufferAllocateInfo allocInfo(commandPool->handle, vk::CommandBufferLevel::ePrimary, (uint32_t)handles.size());

    handles = device->handle.allocateCommandBuffers(allocInfo);

	std::cout << "\tcreated " << handles.size() << " commandbuffers" << std::endl;
}

DerpCommandBuffer::~DerpCommandBuffer()
{
}
