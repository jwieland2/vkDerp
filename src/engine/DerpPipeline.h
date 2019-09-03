#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "DerpDevice.h"
#include "DerpShader.h"
#include "DerpSwapChain.h"
#include "DerpRenderPass.h"
#include "DerpDescriptorSetLayout.h"
//#include "ResourceManager.h"

class DerpPipeline
{
public:
    DerpPipeline(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpSwapChain>& swapChain, std::unique_ptr<DerpRenderPass>& renderPass, std::unique_ptr<DerpDescriptorSetLayout>& descriptorSetLayout);
    ~DerpPipeline();

    std::unique_ptr<DerpShader> vertShader;
    std::unique_ptr<DerpShader> fragShader;

    vk::ShaderModule vertShaderModule;
    vk::ShaderModule fragShaderModule;

	vk::Pipeline handle;
	vk::PipelineLayout layout;
};

