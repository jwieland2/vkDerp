#include "DerpFramebuffers.h"



DerpFramebuffers::DerpFramebuffers(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain, std::unique_ptr<DerpImage>& depthBuffer, std::unique_ptr<DerpRenderPass>& renderPass)
{
	std::cout << "create framebuffers" << std::endl;

	handles.resize(swapChain->imageViews.size());

    for (size_t i = 0; i < swapChain->imageViews.size(); i++)
    {
		vk::ImageView attachments[] = {
			swapChain->imageViews[i],
			depthBuffer->view
        };

        vk::FramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.renderPass =		renderPass->handle;
            framebufferInfo.attachmentCount =	(uint32_t)_countof(attachments);
            framebufferInfo.pAttachments =		attachments;
            framebufferInfo.width =				swapChain->extent.width;
            framebufferInfo.height =			swapChain->extent.height;
            framebufferInfo.layers =			1;

        handles[i] = device->handle.createFramebuffer(framebufferInfo);
    }
}


DerpFramebuffers::~DerpFramebuffers()
{
}
