#include "DerpRenderer.h"

#include "game.h"

#include <stb/stb_image.h>

//#include "data.h"

// Encapsulate height map data for easy sampling
struct HeightMap
{
private:
	uint8_t *heightdata;
	uint32_t dim;
	uint32_t scale;
	int texWidth, texHeight, texChannels;
public:
	HeightMap(std::string filename, uint32_t patchsize)
	{


		unsigned char* pixels = stbi_load("textures/tess/eu4gray.png", &texWidth, &texHeight, &texChannels, 0);
		//unsigned char* pixels = stbi_load("textures/tess/terrain_heightmap_r16.ktx", &texWidth, &texHeight, &texChannels, 4);
		std::cout << "width=" << texWidth << " height=" << texHeight << " nrComponents=" << texChannels << std::endl;
		for (int i = 0; i < 20; i++)
		{
			int p = *(pixels + i);
			std::cout << std::hex << p << std::endl;
		}
		//gli::texture2d heightTex(gli::load(filename));
		//dim = static_cast<uint32_t>(heightTex.extent().x);
		//heightdata = new uint16_t[dim * dim];
		heightdata = new uint8_t[texWidth * texHeight];
		memcpy(heightdata, pixels, texWidth * texHeight * sizeof(uint8_t));
		dim = texWidth;
		this->scale = texWidth / patchsize;
	};

	~HeightMap()
	{
		delete[] heightdata;
	}

	float getHeight(uint32_t x, uint32_t y)
	{
		glm::ivec2 rpos = glm::ivec2(x, y) * glm::ivec2(scale);
		//rpos.x = std::max(0, std::min(rpos.x, (int)dim - 1));
		//rpos.y = std::max(0, std::min(rpos.y, (int)dim - 1));
		rpos.x = std::max(0, std::min(rpos.x, (int)dim - 1));
		rpos.y = std::max(0, std::min(rpos.y, (int)dim - 1));
		rpos /= glm::ivec2(scale);
		return *(heightdata + (rpos.x + rpos.y * dim) * scale) / 65535.0f;
	}
};


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
	uniformBuffer	= std::make_unique<DerpBufferUniform>(device, allocator);

	// rendering
	descriptorPool	= std::make_unique<DerpDescriptorPool>(device, swapChain);
	descriptorSet	= std::make_unique<DerpDescriptorSet>(device, swapChain, descriptorSetLayout, descriptorPool, uniformBuffer, texture, sampler);
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

	pipeline = std::make_unique<DerpPipeline>(device, swapChain, renderPass, descriptorSetLayout);
	std::cout << "\t++pipeline" << std::endl;

	depthBuffer = std::make_unique<DerpImage>();
	depthBuffer->createDepthBuffer(physicalDevice, device, swapChain, allocator);
	std::cout << "\t++depthBuffer" << std::endl;

	framebuffers = std::make_unique<DerpFramebuffers>(device, swapChain, depthBuffer, renderPass);
	std::cout << "\t++framebuffers" << std::endl;

	descriptorPool = std::make_unique<DerpDescriptorPool>(device, swapChain);
	std::cout << "\t++descriptorPool" << std::endl;

	descriptorSet = std::make_unique<DerpDescriptorSet>(device, swapChain, descriptorSetLayout, descriptorPool, uniformBuffer, texture, sampler);
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
	std::cout << "\t--terrainVertex" << std::endl;
	vmaDestroyBuffer(allocator, terrainVertexBuffer->buffer, terrainVertexBuffer->allocation);
	std::cout << "\t--terrainIndex" << std::endl;
	vmaDestroyBuffer(allocator, terrainIndexBuffer->buffer, terrainIndexBuffer->allocation);
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



	uboTess.projection = proj;
	uboTess.modelview = view * glm::mat4(1.0f);
	uboTess.viewportDim = glm::vec2(swapChain->extent.width, swapChain->extent.height);

	frustum.update(uboTess.projection * uboTess.modelview);
	memcpy(uboTess.frustumPlanes, frustum.planes.data(), sizeof(glm::vec4) * 6);

	memcpy(uniformBuffer->data, &uboTess, sizeof(uboTess));
}

// draw simple vertices
void DerpRenderer::drawObject(glm::mat4 model, DerpBufferLocal* inBuffer)
{
	matrixToPush.mvp = viewproj * model;
	cmd->pushConstants(pipeline->layout, vk::ShaderStageFlagBits::eVertex, 0, sizeof(mvp4), &matrixToPush);

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

// Generate a terrain quad patch for feeding to the tessellation control shader
void DerpRenderer::generateTerrain()
{

#define PATCH_SIZE 128
#define UV_SCALE 1.0f

	const uint32_t vertexCount = PATCH_SIZE * PATCH_SIZE;
	std::vector<Vertex> vertices;
	vertices.resize(vertexCount);

	const float wx = 2.0f;
	const float wy = 2.0f;

	for (auto x = 0; x < PATCH_SIZE; x++)
	{
		for (auto y = 0; y < PATCH_SIZE; y++)
		{
			uint32_t index = (x + y * PATCH_SIZE);
			vertices[index].pos.x = x * wx + wx / 2.0f - (float)PATCH_SIZE * wx / 2.0f;
			vertices[index].pos.y = 0.0f;
			vertices[index].pos.z = y * wy + wy / 2.0f - (float)PATCH_SIZE * wy / 2.0f;
			vertices[index].texCoord = glm::vec2((float)x / PATCH_SIZE, (float)y / PATCH_SIZE) * UV_SCALE;
		}
	}

	// Calculate normals from height map using a sobel filter
	HeightMap heightMap("terrain_heightmap_r16.ktx", PATCH_SIZE);

	for (auto x = 0; x < PATCH_SIZE; x++)
	{
		for (auto y = 0; y < PATCH_SIZE; y++)
		{
			// Get height samples centered around current position
			float heights[3][3];
			for (auto hx = -1; hx <= 1; hx++)
			{
				for (auto hy = -1; hy <= 1; hy++)
				{
					heights[hx + 1][hy + 1] = heightMap.getHeight(x + hx, y + hy);
				}
			}

			// Calculate the normal
			glm::vec3 normal;
			// Gx sobel filter
			normal.x = heights[0][0] - heights[2][0] + 2.0f * heights[0][1] - 2.0f * heights[2][1] + heights[0][2] - heights[2][2];
			// Gy sobel filter
			normal.z = heights[0][0] + 2.0f * heights[1][0] + heights[2][0] - heights[0][2] - 2.0f * heights[1][2] - heights[2][2];
			// Calculate missing up component of the normal using the filtered x and y axis
			// The first value controls the bump strength
			normal.y = 0.25f * sqrt(1.0f - normal.x * normal.x - normal.z * normal.z);

			vertices[x + y * PATCH_SIZE].normal = glm::normalize(normal * glm::vec3(2.0f, 1.0f, 2.0f));
		}
	}

	// Indices
	const uint32_t w = (PATCH_SIZE - 1);
	const uint32_t indexCount = w * w * 4;
	std::vector<uint16_t> indices;
	indices.resize(indexCount);
	for (auto x = 0; x < w; x++)
	{
		for (auto y = 0; y < w; y++)
		{
			uint32_t index = (x + y * w) * 4;
			indices[index] = (x + y * PATCH_SIZE);
			indices[index + 1] = indices[index] + PATCH_SIZE;
			indices[index + 2] = indices[index + 1] + 1;
			indices[index + 3] = indices[index] + 1;
		}
	}
	//models.terrain.indexCount = indexCount;

	uint32_t vertexBufferSize = vertexCount * sizeof(Vertex);
	uint32_t indexBufferSize = indexCount * sizeof(uint32_t);

	//struct {
	//	VkBuffer buffer;
	//	VkDeviceMemory memory;
	//} vertexStaging, indexStaging;

	// Create staging buffers

	terrainVertexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
	terrainIndexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, indices, allocator);

	//VK_CHECK_RESULT(vulkanDevice->createBuffer(
	//	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	//	vertexBufferSize,
	//	&vertexStaging.buffer,
	//	&vertexStaging.memory,
	//	vertices));

	//VK_CHECK_RESULT(vulkanDevice->createBuffer(
	//	VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
	//	VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
	//	indexBufferSize,
	//	&indexStaging.buffer,
	//	&indexStaging.memory,
	//	indices));

	//VK_CHECK_RESULT(vulkanDevice->createBuffer(
	//	VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
	//	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	//	vertexBufferSize,
	//	&models.terrain.vertices.buffer,
	//	&models.terrain.vertices.memory));

	//VK_CHECK_RESULT(vulkanDevice->createBuffer(
	//	VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
	//	VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	//	indexBufferSize,
	//	&models.terrain.indices.buffer,
	//	&models.terrain.indices.memory));

	//// Copy from staging buffers
	//VkCommandBuffer copyCmd = VulkanExampleBase::createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

	//VkBufferCopy copyRegion = {};

	//copyRegion.size = vertexBufferSize;
	//vkCmdCopyBuffer(
	//	copyCmd,
	//	vertexStaging.buffer,
	//	models.terrain.vertices.buffer,
	//	1,
	//	&copyRegion);

	//copyRegion.size = indexBufferSize;
	//vkCmdCopyBuffer(
	//	copyCmd,
	//	indexStaging.buffer,
	//	models.terrain.indices.buffer,
	//	1,
	//	&copyRegion);

	//VulkanExampleBase::flushCommandBuffer(copyCmd, queue, true);

	//models.terrain.device = device;

	//vkDestroyBuffer(device, vertexStaging.buffer, nullptr);
	//vkFreeMemory(device, vertexStaging.memory, nullptr);
	//vkDestroyBuffer(device, indexStaging.buffer, nullptr);
	//vkFreeMemory(device, indexStaging.memory, nullptr);

	//delete[] vertices;
	//delete[] indices;
}

