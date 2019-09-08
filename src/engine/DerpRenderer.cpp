#include "DerpRenderer.h"

#include "game.h"



DerpRenderer::DerpRenderer()
{
	initWindow();
	//fpsMonitor = FpsMonitor();
	initVulkan();
}


DerpRenderer::~DerpRenderer()
{
	cleanup();
}

void DerpRenderer::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void DerpRenderer::initVma()
{
	VmaAllocatorCreateInfo createInfo = {};
	createInfo.physicalDevice = physicalDevice->handle;
	createInfo.device = device->handle;

	vmaCreateAllocator(&createInfo, &allocator);
}

void DerpRenderer::initVulkan()
{
	// context
	instance		= std::make_unique<DerpInstance>();
	surface			= std::make_unique<DerpSurface>(instance, window);
	physicalDevice  = std::make_unique<DerpPhysicalDevice>(instance, surface);
	device			= std::make_unique<DerpDevice>(physicalDevice);

	// memory management
	initVma();

	// graphics
	swapChain		= std::make_unique<DerpSwapChain>(window, surface, physicalDevice, device);
	depthBuffer		= std::make_unique<DerpImage>();
	depthBuffer->createDepthBuffer(physicalDevice, device, swapChain, allocator);
	renderPass		= std::make_unique<DerpRenderPass>(device, swapChain);
	framebuffers	= std::make_unique<DerpFramebuffers>(device, swapChain, depthBuffer, renderPass);
	descriptorSetLayout = std::make_unique<DerpDescriptorSetLayout>(device);
	pipeline		= std::make_unique<DerpPipeline>(device, swapChain, renderPass, descriptorSetLayout);
	commandPool		= std::make_unique<DerpCommandPool>(physicalDevice, device);

	// buffers
	texture			= std::make_unique<DerpImage>();
	texture->createTexture(device, commandPool, allocator);
	sampler			= std::make_unique<DerpSampler>(device);
	//vertexBuffer	= std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
	indexBuffer		= std::make_unique<DerpBufferLocal>(device, commandPool, indices, allocator);
	uniformBuffer	= std::make_unique<DerpBufferUniform>(device, swapChain, allocator);

	// rendering
	descriptorPool	= std::make_unique<DerpDescriptorPool>(device, swapChain);
	descriptorSet	= std::make_unique<DerpDescriptorSet>(device, swapChain, descriptorSetLayout, descriptorPool, uniformBuffer, texture, sampler);
	commandBuffers	= std::make_unique<DerpCommandBuffer>(device, commandPool, framebuffers, p4);
	sync			= std::make_unique<DerpSync>(device);
}


void DerpRenderer::cleanupSwapChain()
{
	std::cout << "cleanup swapChain" << std::endl;

	device->handle.waitIdle();

	std::cout << "\t--commandbuffers" << std::endl;
	device->handle.freeCommandBuffers(commandPool->handle, commandBuffers->handles);

	std::cout << "\t--renderpass" << std::endl;
	device->handle.destroyRenderPass(renderPass->handle);
	renderPass.reset();

	std::cout << "\t--pipelinelayout" << std::endl;
	device->handle.destroyPipelineLayout(pipeline->layout);

	std::cout << "\t--pipeline" << std::endl;
	device->handle.destroyPipeline(pipeline->handle);
	pipeline.reset();

	std::cout << "\t--framebuffers" << std::endl;
	for (auto framebuffer : framebuffers->handles) {
		device->handle.destroyFramebuffer(framebuffer, nullptr);
	}
	framebuffers.reset();

	std::cout << "\t--depthBuffer" << std::endl;
	device->handle.destroyImageView(depthBuffer->view);
	depthBuffer.reset();

	for (auto imgViews : swapChain->imageViews)
	{
		std::cout << "\t--imageviews " << std::endl;
		device->handle.destroyImageView(imgViews);
	}

	std::cout << "\t--swapchain" << std::endl;
	device->handle.destroySwapchainKHR(swapChain->handle);
	swapChain.reset();

	std::cout << "\t--sync" << std::endl;
	device->handle.destroySemaphore(sync->imageAvailableSemaphore);
	device->handle.destroySemaphore(sync->renderFinishedSemaphore);
	for (size_t i = 0; i < sync->maxFramesInFlight; i++) {
		device->handle.destroyFence(sync->inFlightFences[i], nullptr);
	}
	sync.reset();

	std::cout << "\t--descriptorPool" << std::endl;
	device->handle.destroyDescriptorPool(descriptorPool->handle);
}

void DerpRenderer::recreateSwapChain()
{
	int width = 0, height = 0;
	while (width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}

	std::cout << "recreate swapChain - new window size: " << width << "x" << height << std::endl;

	device->handle.waitIdle();

	swapChain = std::make_unique<DerpSwapChain>(window, surface, physicalDevice, device);
	//std::cout << "++swapchain" << std::endl;

	renderPass = std::make_unique<DerpRenderPass>(device, swapChain);
	//std::cout << "++renderpass" << std::endl;

	pipeline = std::make_unique<DerpPipeline>(device, swapChain, renderPass, descriptorSetLayout);
	//std::cout << "++pipeline" << std::endl;

	depthBuffer = std::make_unique<DerpImage>();
	depthBuffer->createDepthBuffer(physicalDevice, device, swapChain, allocator);
	//std::cout << "++depthBuffer" << std::endl;

	framebuffers = std::make_unique<DerpFramebuffers>(device, swapChain, depthBuffer, renderPass);
	//std::cout << "++framebuffers" << std::endl;

	descriptorPool = std::make_unique<DerpDescriptorPool>(device, swapChain);
	//std::cout << "++descriptorPool" << std::endl;

	descriptorSet = std::make_unique<DerpDescriptorSet>(device, swapChain, descriptorSetLayout, descriptorPool, uniformBuffer, texture, sampler);
	//std::cout << "++descriptorSet" << std::endl;

	commandBuffers = std::make_unique<DerpCommandBuffer>(device, commandPool, framebuffers, p4);
	//std::cout << "++commandbuffers" << std::endl;

	sync = std::make_unique<DerpSync>(device);
	//std::cout << "++sync" << std::endl;
}

void DerpRenderer::cleanup()
{
	std::cout << "--depth image" << std::endl;
	vmaDestroyImage(allocator, depthBuffer->handle, depthBuffer->allocation);

	std::cout << "--swapchain" << std::endl;
	cleanupSwapChain();

	std::cout << "--sampler" << std::endl;
	device->handle.destroySampler(sampler->handle);

	std::cout << "--texture view" << std::endl;
	device->handle.destroyImageView(texture->view);

	std::cout << "--descriptorSetLayout" << std::endl;
	device->handle.destroyDescriptorSetLayout(descriptorSetLayout->handle);

	std::cout << "--commandPool" << std::endl;
	device->handle.destroyCommandPool(commandPool->handle);

	std::cout << "--debugMessenger" << std::endl;
	if (enableValidationLayers)
		instance->handle.destroyDebugUtilsMessengerEXT(instance->debugMessenger, nullptr, instance->dynamicLoader);

	std::cout << "--surface" << std::endl;
	instance->handle.destroySurfaceKHR(surface->handle.get());
	surface.release();

	std::cout << "--Vma" << std::endl;

	std::cout << "\t--tex image" << std::endl;
	vmaDestroyImage(allocator, texture->handle, texture->allocation);
	std::cout << "\t--vertex buffer" << std::endl;
	vmaDestroyBuffer(allocator, vertexBuffer->buffer, vertexBuffer->allocation);
	std::cout << "\t--index buffer" << std::endl;
	vmaDestroyBuffer(allocator, indexBuffer->buffer, indexBuffer->allocation);
	std::cout << "\t--uniform buffer" << std::endl;
	vmaDestroyBuffer(allocator, uniformBuffer->uniformBuffers[0], uniformBuffer->bufferAllocation);
	std::cout << "\t--allocator" << std::endl;
	vmaDestroyAllocator(allocator);

	std::cout << "--device" << std::endl;
	device->handle.destroy();

	std::cout << "--instance" << std::endl;
	instance->handle.destroy();
}

void DerpRenderer::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
	app->renderer->framebufferResized = true;
}

void DerpRenderer::drawFrame(Camera* camera)
{
	// calculate framerate
	fpsMonitor.update();
	fpsMonitor.updateWindow(window, 150.0);
	//if (fpsMonitor.timeSinceUpdate != 0.0)
	//	return;

	vk::Result result;

	size_t cmdIndex = (nextCommandBufferIndex++) % sync->maxFramesInFlight;
	vk::CommandBuffer cmd = commandBuffers->handles[cmdIndex];

	device->handle.waitForFences(1, &sync->inFlightFences[cmdIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());


	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	beginInfo.pInheritanceInfo = nullptr; // Optional


	// begin recording
	cmd.begin(beginInfo);

	uint32_t imageIndex = 0;
	result = device->handle.acquireNextImageKHR(swapChain->handle, std::numeric_limits<uint64_t>::max(), sync->imageAvailableSemaphore, nullptr, &imageIndex);

	// resize check
	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized)
	{
		std::cout << "resized!" << std::endl;
		framebufferResized = false;
		cleanupSwapChain();
		recreateSwapChain();
		device->handle.waitIdle();
		return;

	}

	// background color
	std::array<vk::ClearValue, 2> clearValues;
	clearValues[0].color.float32[0] = 0.6f;
	clearValues[0].color.float32[1] = 0.6f;
	clearValues[0].color.float32[2] = 0.6f;
	clearValues[0].color.float32[3] = 1.0f;
	clearValues[1].depthStencil.depth = 1.0f;

	vk::RenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.renderPass = renderPass->handle;
	renderPassInfo.framebuffer = framebuffers->handles[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->extent;
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	cmd.beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

	cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->handle);

	// view projection matrix
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (float)swapChain->extent.width / (float)swapChain->extent.height, 0.1f, 1000.0f);
	proj[1][1] *= -1;

	// push constants
	p4.mvp = proj * view;
	cmd.pushConstants(pipeline->layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4), &p4);

	// update uniforms
	bufferColor.uColor = glm::vec4(0.5 + 0.5*sin(glfwGetTime()*1.479), 0.5 + 0.5*sin(glfwGetTime()), 0.5 + 0.5*sin(glfwGetTime()*2.3), 1.0f);
	*(uniformBuffer->data) = bufferColor;

	// bind vertex and index Buffers
	std::vector<vk::DeviceSize> offsets = { 0 };
		
	cmd.bindVertexBuffers(0, vertexBuffer->buffer, offsets);
	cmd.bindIndexBuffer(indexBuffer->buffer, 0, vk::IndexType::eUint16);

	// bind descriptorsets
	cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->layout, 0, 1, &descriptorSet->handle, 0, nullptr);

	//cmd.drawIndexed(indexBuffer->num, 1, 0, 0, 0);	// indexed vertices

	cmd.draw(vertexBuffer->num, 1, 0, 0);

	// end recording
	cmd.endRenderPass();
	cmd.end();

	// submit to queue
	vk::PipelineStageFlags waitStages = vk::PipelineStageFlagBits::eColorAttachmentOutput;

	vk::SubmitInfo submitInfo(
		1,
		&sync->imageAvailableSemaphore,
		&waitStages,
		1,
		&commandBuffers->handles[cmdIndex],
		1,
		&sync->renderFinishedSemaphore
	);

	device->handle.resetFences(1, &sync->inFlightFences[cmdIndex]);

	result = device->graphicsQueue.submit(1, &submitInfo, sync->inFlightFences[cmdIndex]);

	// present
	result = device->presentQueue.presentKHR(
		vk::PresentInfoKHR(
			1,
			&sync->renderFinishedSemaphore,
			1,
			&swapChain->handle,
			&imageIndex,
			nullptr
		));
}