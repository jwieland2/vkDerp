#include "DerpCommandBuffer.h"



DerpCommandBuffer::DerpCommandBuffer(
	std::unique_ptr<DerpDevice>& device,
	std::unique_ptr<DerpCommandPool>& commandPool,
	std::unique_ptr<DerpFramebuffers>& framebuffers)
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
