#include "DerpRenderer.h"

#include "game.h"


DerpRenderer::DerpRenderer()
{
	initWindow();
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
	// init structs
	DerpPipelineInit basicPipelineInit;
	{
		basicPipelineInit.vertPath = "shaders/vert.spv";
		basicPipelineInit.fragPath = "shaders/frag.spv";
		basicPipelineInit.pushSize = sizeof(uint32_t);
		basicPipelineInit.pushStage = vk::ShaderStageFlagBits::eFragment;
		basicPipelineInit.hasUniform = true;
		basicPipelineInit.hasPushConstants = true;
	}

	DerpImageInit headInit;
	{
		headInit.path = "textures/texture.jpg";
	}

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
	pipeline		= std::make_unique<DerpPipeline>(device, swapChain, renderPass, descriptorSetLayout, basicPipelineInit);
	commandPool		= std::make_unique<DerpCommandPool>(physicalDevice, device);

	// buffers
	texture			= std::make_unique<DerpImage>();
	texture->createTexture(headInit, device, commandPool, allocator);
	sampler			= std::make_unique<DerpSampler>(DerpSamplerInit(), device);
	uniformBuffer	= std::make_unique<DerpBufferUniform>(device, allocator);

	// rendering
	descriptorPool	= std::make_unique<DerpDescriptorPool>(device, swapChain);

	DerpDescriptorInit descriptorInit;
		descriptorInit.sampler = &(sampler->handle);
		descriptorInit.textureView = &(texture->view);
	descriptorSet	= std::make_unique<DerpDescriptorSet>(descriptorInit, device, swapChain, descriptorSetLayout, descriptorPool, uniformBuffer);
	commandBuffers	= std::make_unique<DerpCommandBuffer>(device, commandPool, framebuffers);
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
	std::cout << "\t++swapchain" << std::endl;

	renderPass = std::make_unique<DerpRenderPass>(device, swapChain);
	std::cout << "\t++renderpass" << std::endl;

	DerpPipelineInit basicPipelineInit;
	{
		basicPipelineInit.vertPath = "shaders/vert.spv";
		basicPipelineInit.fragPath = "shaders/frag.spv";
		basicPipelineInit.pushSize = sizeof(uint8_t);
		basicPipelineInit.pushStage = vk::ShaderStageFlagBits::eFragment;
		basicPipelineInit.hasUniform = true;
		basicPipelineInit.hasPushConstants = true;
	}

	pipeline = std::make_unique<DerpPipeline>(device, swapChain, renderPass, descriptorSetLayout, basicPipelineInit);
	std::cout << "\t++pipeline" << std::endl;

	depthBuffer = std::make_unique<DerpImage>();
	depthBuffer->createDepthBuffer(physicalDevice, device, swapChain, allocator);
	std::cout << "\t++depthBuffer" << std::endl;

	framebuffers = std::make_unique<DerpFramebuffers>(device, swapChain, depthBuffer, renderPass);
	std::cout << "\t++framebuffers" << std::endl;

	descriptorPool = std::make_unique<DerpDescriptorPool>(device, swapChain);
	std::cout << "\t++descriptorPool" << std::endl;

	DerpDescriptorInit descriptorInit;
	descriptorInit.sampler = &(sampler->handle);
	descriptorInit.textureView = &(texture->view);

	descriptorSet = std::make_unique<DerpDescriptorSet>(descriptorInit, device, swapChain, descriptorSetLayout, descriptorPool, uniformBuffer);
	std::cout << "\t++descriptorSet" << std::endl;

	commandBuffers = std::make_unique<DerpCommandBuffer>(device, commandPool, framebuffers);
	std::cout << "\t++commandbuffers" << std::endl;

	sync = std::make_unique<DerpSync>(device);
	std::cout << "\t++sync" << std::endl;
}

void DerpRenderer::cleanup()
{
	std::cout << "--depth image" << std::endl;
	vmaDestroyImage(allocator, depthBuffer->handle, depthBuffer->allocation);

	std::cout << "--swapchain" << std::endl;
	cleanupSwapChain();

	std::cout << "--sampler" << std::endl;
	device->handle.destroySampler(sampler->handle);

	//std::cout << "--texture view" << std::endl;
	//device->handle.destroyImageView(texture->view);

	//std::cout << "--height view" << std::endl;
	//device->handle.destroyImageView(heightmap->view);

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
	//std::cout << "\t--height image" << std::endl;
	//vmaDestroyImage(allocator, heightmap->handle, heightmap->allocation);
	//std::cout << "\t--terrainVertex" << std::endl;
	//vmaDestroyBuffer(allocator, terrainVertexBuffer->buffer, terrainVertexBuffer->allocation);
	//std::cout << "\t--terrainIndex" << std::endl;
	//vmaDestroyBuffer(allocator, terrainIndexBuffer->buffer, terrainIndexBuffer->allocation);
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

void DerpRenderer::beginDraw(Camera* camera)
{
	// calculate framerate
	fpsMonitor.update();
	fpsMonitor.updateWindow(window, 150.0);
	//if (fpsMonitor.timeSinceUpdate != 0.0)
	//	return;

	vk::Result result;

	cmdIndex = (nextCommandBufferIndex++) % sync->maxFramesInFlight;
	cmd = &(commandBuffers->handles[cmdIndex]);

	device->handle.waitForFences(1, &sync->inFlightFences[cmdIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());


	vk::CommandBufferBeginInfo beginInfo = {};
	beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
	beginInfo.pInheritanceInfo = nullptr; // Optional


	// begin recording
	cmd->begin(beginInfo);

	imageIndex = 0;
	result = device->handle.acquireNextImageKHR(swapChain->handle, std::numeric_limits<uint64_t>::max(), sync->imageAvailableSemaphore, nullptr, &imageIndex);

	// resize check
	if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized)
	{
		std::cout << "resized!" << std::endl;
		framebufferResized = false;
		cleanupSwapChain();
		recreateSwapChain();
		device->handle.waitIdle();
		nextCommandBufferIndex = 0;
		imageIndex = 0;
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

	cmd->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

	cmd->bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->handle);

	// bind descriptorsets
	cmd->bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline->layout, 0, 1, &descriptorSet->handle, 0, nullptr);

	//cmd->bindIndexBuffer(indexBuffer->buffer, 0, vk::IndexType::eUint16);

	// view projection matrix
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 proj = glm::perspective(glm::radians(camera->fov), (float)swapChain->extent.width / (float)swapChain->extent.height, 0.1f, 1000.0f);
	//proj[1][1] *= -1;
	viewproj = proj * view;



	//uboTess.projection = proj;
	//uboTess.modelview = view * glm::mat4(1.0f);
	//uboTess.viewportDim = glm::vec2(swapChain->extent.width, swapChain->extent.height);

	//frustum.update(uboTess.projection * uboTess.modelview);
	//memcpy(uboTess.frustumPlanes, frustum.planes.data(), sizeof(glm::vec4) * 6);

	//memcpy(uniformBuffer->data, &uboTess, sizeof(uboTess));

	
}

// draw simple vertices
void DerpRenderer::drawObject(glm::mat4 model, DerpBufferLocal* inBuffer)
{
	matrixToPush.mvp = viewproj * model;
	memcpy(uniformBuffer->data, &viewproj, sizeof(glm::mat4));

	uint8_t texIndex = 1;
	cmd->pushConstants(pipeline->layout, vk::ShaderStageFlagBits::eFragment, 0, sizeof(uint8_t), &texIndex);

	std::vector<vk::Buffer> buf = { inBuffer->buffer };
	cmd->bindVertexBuffers(0, buf, { 0 });

	cmd->draw(inBuffer->num, 1, 0, 0);
}


// indexed vertex drawing
void DerpRenderer::drawObject(glm::mat4 model, DerpBufferLocal* inBuffer, DerpBufferLocal* indexBuffer)
{
	matrixToPush.mvp = viewproj * model;
	cmd->pushConstants(pipeline->layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(mvp4), &matrixToPush);

	std::vector<vk::Buffer> vertex = { inBuffer->buffer };
	cmd->bindVertexBuffers(0, vertex, { 0 });
	cmd->bindIndexBuffer(indexBuffer->buffer, 0, vk::IndexType::eUint16);

	cmd->drawIndexed(indexBuffer->num, 1, 0, 0, 0);
}

void DerpRenderer::endDraw()
{
	// end recording
	cmd->endRenderPass();
	cmd->end();

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

	vk::Result result = device->graphicsQueue.submit(1, &submitInfo, sync->inFlightFences[cmdIndex]);

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