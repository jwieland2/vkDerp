#include "DerpPipeline.h"

#include "Vertex.h"

DerpPipeline::DerpPipeline(std::unique_ptr<DerpDevice>& device,
		std::unique_ptr<DerpSwapChain>& swapChain,
		std::unique_ptr<DerpRenderPass>& renderPass,
		std::unique_ptr<DerpDescriptorSetLayout>& descriptorSetLayout)
{
	std::cout << "create pipeline" << std::endl;
    vertShader = std::make_unique<DerpShader>("shaders/vert.spv");
    fragShader = std::make_unique<DerpShader>("shaders/frag.spv");

    vertShaderModule = vertShader->createModule(device->handle);
    fragShaderModule = fragShader->createModule(device->handle);

	    vk::PipelineShaderStageCreateInfo shaderStages[] = {
	        {{}, vk::ShaderStageFlagBits::eVertex, vertShaderModule, "main"},
	        {{}, vk::ShaderStageFlagBits::eFragment, fragShaderModule, "main"}
	    };

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo({}, 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly({}, vk::PrimitiveTopology::eTriangleList, VK_FALSE);

    vk::Viewport viewport(0.0f, 0.0f, (float)swapChain->extent.width, (float)swapChain->extent.height, 0.0f, 1.0f);

    vk::Rect2D scissor({ 0, 0 }, swapChain->extent);

    vk::PipelineViewportStateCreateInfo viewportState({}, 1, &viewport, 1, &scissor);

	vk::PipelineDepthStencilStateCreateInfo depthStencil = vk::PipelineDepthStencilStateCreateInfo().
		setDepthTestEnable(VK_TRUE).
		setDepthWriteEnable(VK_TRUE).
		setDepthCompareOp(vk::CompareOp::eLess).
		setDepthBoundsTestEnable(VK_FALSE).
		setStencilTestEnable(VK_FALSE);

	vk::PipelineRasterizationStateCreateInfo rasterizer({}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill, vk::CullModeFlagBits::eNone, vk::FrontFace::eClockwise, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);

	vk::PipelineMultisampleStateCreateInfo multisampling({}, vk::SampleCountFlagBits::e1, VK_FALSE, 1.0f, nullptr, VK_FALSE, VK_FALSE);

	vk::PipelineColorBlendAttachmentState colorBlendAttachment(
        VK_FALSE,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
    );

    vk::PipelineColorBlendStateCreateInfo colorBlending(
        {},
        VK_FALSE,
        vk::LogicOp::eCopy,
        1,
        &colorBlendAttachment
    );

	vk::PushConstantRange pushRange(vk::ShaderStageFlagBits::eVertex, 0, sizeof(glm::mat4));

	//vk::PipelineLayoutCreateInfo plci({}, 0, nullptr, 1, &pushRange);	// push only
	//vk::PipelineLayoutCreateInfo plci({}, 1, &descriptorSetLayout->handle, 0, nullptr);	// uniform only
	vk::PipelineLayoutCreateInfo plci({}, 1, &descriptorSetLayout->handle, 1, &pushRange); // uniform+push
	//vk::PipelineLayoutCreateInfo plci({}, 0, nullptr, 0, nullptr); // no uniform no push

    layout = device->handle.createPipelineLayout(plci);

    vk::GraphicsPipelineCreateInfo gpci = {};
        gpci.stageCount = 2;
        gpci.pStages = shaderStages;
        gpci.pVertexInputState = &vertexInputInfo;
		gpci.pInputAssemblyState = &inputAssembly;
        gpci.pViewportState = &viewportState;
        gpci.pRasterizationState = &rasterizer;
        gpci.pMultisampleState = &multisampling;
        gpci.pDepthStencilState = &depthStencil;
        gpci.pColorBlendState = &colorBlending;
        gpci.pDynamicState = nullptr;
        gpci.layout = layout;
		gpci.renderPass = renderPass->handle;
        gpci.subpass = 0;

    handle = device->handle.createGraphicsPipeline(nullptr, gpci);
    
    device->handle.destroyShaderModule(vertShaderModule);
    device->handle.destroyShaderModule(fragShaderModule);
}


DerpPipeline::~DerpPipeline()
{
}
