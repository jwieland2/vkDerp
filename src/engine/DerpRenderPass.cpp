#include "DerpRenderPass.h"

#include <stdio.h>


DerpRenderPass::DerpRenderPass(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain)
{
	std::cout << "create renderPass" << std::endl;

	vk::AttachmentDescription attachments[2];

	attachments[0].setFormat(swapChain->imageFormat);
	attachments[0].setSamples(vk::SampleCountFlagBits::e1);
	attachments[0].setLoadOp(vk::AttachmentLoadOp::eClear);
	attachments[0].setStoreOp(vk::AttachmentStoreOp::eStore);
	attachments[0].setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	attachments[0].setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	attachments[0].setInitialLayout(vk::ImageLayout::eUndefined);
	attachments[0].setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

	attachments[1].setFormat(swapChain->depthFormat);
	attachments[1].setSamples(vk::SampleCountFlagBits::e1);
	attachments[1].setLoadOp(vk::AttachmentLoadOp::eClear);
	attachments[1].setStoreOp(vk::AttachmentStoreOp::eDontCare);
	attachments[1].setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
	attachments[1].setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
	attachments[1].setInitialLayout(vk::ImageLayout::eUndefined);
	attachments[1].setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);    // layout(location = 0) out vec4 outColor
	vk::AttachmentReference depthStencilAttachment(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

	vk::SubpassDescription subpass = vk::SubpassDescription().
		setPipelineBindPoint(vk::PipelineBindPoint::eGraphics).
		setColorAttachmentCount(1).
		setPColorAttachments(&colorAttachmentRef).
		setPDepthStencilAttachment(&depthStencilAttachment);
	
	vk::RenderPassCreateInfo createInfo = vk::RenderPassCreateInfo().
		setAttachmentCount((uint32_t)_countof(attachments)).
		setPAttachments(attachments).
		setSubpassCount(1).
		setPSubpasses(&subpass).
		setDependencyCount(0);

	handle = device->handle.createRenderPass(createInfo);
}


DerpRenderPass::~DerpRenderPass()
{
}
