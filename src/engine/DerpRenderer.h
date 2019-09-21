#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "vk_mem_alloc.h"
#include "FpsMonitor.h"
#include "Vertex.h"

#include "DerpInstance.h"
#include "DerpSurface.h"
#include "DerpPhysicalDevice.h"
#include "DerpDevice.h"
#include "DerpSwapChain.h"
#include "DerpDescriptorSetLayout.h"
#include "DerpPipeline.h"
#include "DerpFramebuffers.h"
#include "DerpCommandPool.h"
#include "DerpBufferLocal.h"
#include "DerpImage.h"
#include "DerpSampler.h"
#include "DerpBufferUniform.h"
#include "DerpDescriptorPool.h"
#include "DerpDescriptorSet.h"
#include "DerpCommandBuffer.h"
#include "DerpSync.h"

#include "Camera.h"
#include "frustum.hpp"

const int WIDTH = 1400;
const int HEIGHT = 800;
const int COMMAND_BUFFER_COUNT = 2;

class DerpRenderer
{
public:
	DerpRenderer();
	~DerpRenderer();

	std::unique_ptr<DerpInstance> instance;
	GLFWwindow* window;
	std::unique_ptr<DerpSurface> surface;
	std::unique_ptr<DerpPhysicalDevice> physicalDevice;
	std::unique_ptr<DerpDevice> device;
	std::unique_ptr<DerpSwapChain> swapChain;
	VmaAllocator allocator;
	std::unique_ptr<DerpDescriptorSetLayout> descriptorSetLayout;
	std::unique_ptr<DerpPipeline> pipeline;
	std::unique_ptr<DerpRenderPass> renderPass;
	std::unique_ptr<DerpFramebuffers> framebuffers;
	std::unique_ptr<DerpCommandPool> commandPool;
	std::unique_ptr<DerpImage> texture;
	std::unique_ptr<DerpSampler> sampler;
	std::unique_ptr<DerpImage> depthBuffer;
	std::unique_ptr<DerpBufferUniform> uniformBuffer;
	std::unique_ptr<DerpDescriptorPool> descriptorPool;
	std::unique_ptr<DerpDescriptorSet> descriptorSet;
	std::unique_ptr<DerpCommandBuffer> commandBuffers;
	std::unique_ptr<DerpSync> sync;

	// drawing
	vk::CommandBuffer* cmd;
	size_t nextCommandBufferIndex = 0;
	size_t cmdIndex;
	uint32_t imageIndex;
	glm::mat4 viewproj;
	mvp4 matrixToPush;

	UBO uboTess;
	Frustum frustum;

	bool framebufferResized = false;

	FpsMonitor fpsMonitor;

	void initWindow();
	void initVma();
	void initVulkan();

	void cleanup();
	void cleanupSwapChain();
	void recreateSwapChain();
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

	void beginDraw(Camera* camera);
	void drawObject(glm::mat4 model, DerpBufferLocal* inBuffer);
	void drawObject(glm::mat4 model, DerpBufferLocal* inBuffer, DerpBufferLocal* indexBuffer);
	void endDraw();
};