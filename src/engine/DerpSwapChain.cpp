#include "DerpSwapChain.h"



DerpSwapChain::DerpSwapChain(GLFWwindow* window,
    std::unique_ptr<DerpSurface>& surface,
    std::unique_ptr<DerpPhysicalDevice>& physicalDevice,
    std::unique_ptr<DerpDevice>& device)
{
    std::cout << "create swapchain" << std::endl;
	physicalDevice->swapChainSupport.query(physicalDevice->handle, surface->handle);
    DerpPhysicalDevice::SwapChainSupportDetails support = physicalDevice->swapChainSupport;

    vk::SurfaceFormatKHR surfaceFormat = support.formats[0];
    vk::PresentModeKHR presentMode = vk::PresentModeKHR::eFifo;

    // choose swap surface format
    for (const auto& availableFormat : support.formats)
    {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            //return availableFormat;
            surfaceFormat = availableFormat;
            break;
        }
    }

    // choose swap present mode
    for (const auto& availablePresentMode : support.presentModes)
    {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox)
        {
            presentMode = availablePresentMode;
        }
    }

    // choose swap extent
	std::cout << "\tminImageCount= " << support.capabilities.minImageCount << std::endl <<
		"\tmaxImageCount= " << support.capabilities.maxImageCount << std::endl <<
		"\tsupportedTransforms " << vk::to_string(support.capabilities.supportedTransforms) << std::endl <<
		"\tcurrentTransform " << vk::to_string(support.capabilities.currentTransform) << std::endl <<
		"\tsupportedCompositeAlpha " << vk::to_string(support.capabilities.supportedCompositeAlpha) << std::endl <<
		"\tsupportedUsageFlags " << vk::to_string(support.capabilities.supportedUsageFlags) << std::endl;


    if (support.capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
		//std::cout << "currentExtent is not 0xFF: " << support.capabilities.currentExtent.width << std::endl;
        extent = support.capabilities.currentExtent;
    }
    else
    {
		//std::cout << "currentExtent is 0xFF" << std::endl;
        
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

        vk::Extent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::max(
			physicalDevice->swapChainSupport.capabilities.minImageExtent.width,
			std::min(physicalDevice->swapChainSupport.capabilities.maxImageExtent.width, actualExtent.width)
		);

        actualExtent.height = std::max(
			physicalDevice->swapChainSupport.capabilities.minImageExtent.height,
			std::min(physicalDevice->swapChainSupport.capabilities.maxImageExtent.height, actualExtent.height)
		);

        extent = actualExtent;
    }

    uint32_t imageCount = support.capabilities.minImageCount + 1;
	

    if (support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount)
    {
        imageCount = support.capabilities.maxImageCount;
    }

	std::cout << "\timageCount = " << imageCount << std::endl;

    vk::SwapchainCreateInfoKHR scci({}, surface->handle.get());
    scci.minImageCount = imageCount;
    scci.imageFormat = surfaceFormat.format;
    scci.imageColorSpace = surfaceFormat.colorSpace;
    scci.imageExtent = extent;
    scci.imageArrayLayers = 1;
    scci.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
	scci.preTransform = support.capabilities.currentTransform;

    uint32_t queueFamilyIndices[] = { physicalDevice->queueFamilyIndices.graphicsFamily.value(), physicalDevice->queueFamilyIndices.presentFamily.value() };

    if (physicalDevice->queueFamilyIndices.graphicsFamily != physicalDevice->queueFamilyIndices.presentFamily)
    {
        scci.imageSharingMode = vk::SharingMode::eConcurrent;
        scci.queueFamilyIndexCount = 2;
        scci.pQueueFamilyIndices = queueFamilyIndices;
    }

    scci.presentMode = presentMode;
    scci.clipped = VK_TRUE;

    handle = device->handle.createSwapchainKHR(scci);

    images = device->handle.getSwapchainImagesKHR(handle);
    imageFormat = surfaceFormat.format;
	depthFormat = findDepthFormat(physicalDevice);
    extent = extent;

	imageViews.resize(images.size());

    for (size_t i = 0; i < images.size(); i++)
    {
        vk::ImageViewCreateInfo ivci({}, images[i], vk::ImageViewType::e2D, imageFormat);
            ivci.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            ivci.subresourceRange.baseMipLevel = 0;
            ivci.subresourceRange.levelCount = 1;
            ivci.subresourceRange.baseArrayLayer = 0;
            ivci.subresourceRange.layerCount = 1;

        imageViews[i] = device->handle.createImageView(ivci);
    }
}


DerpSwapChain::~DerpSwapChain()
{
}

vk::Format DerpSwapChain::findDepthFormat(std::unique_ptr<DerpPhysicalDevice>& physicalDevice)
{
	std::vector<vk::Format> formats;
	formats.push_back(vk::Format::eD32Sfloat);
	formats.push_back(vk::Format::eD32SfloatS8Uint);
	formats.push_back(vk::Format::eD24UnormS8Uint);

	for (vk::Format format : formats)
	{
		vk::FormatProperties props = physicalDevice->handle.getFormatProperties(format);

		if ((props.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment) == vk::FormatFeatureFlagBits::eDepthStencilAttachment)
		{
			return format;
		}
	}
	return vk::Format::eUndefined;
}
