#include "DerpPipeline.h"

//#include "Vertex.h"

DerpPipeline::DerpPipeline(std::unique_ptr<DerpDevice>& device,std::unique_ptr<DerpSwapChain>& swapChain,std::unique_ptr<DerpRenderPass>& renderPass,std::unique_ptr<DerpDescriptorSetLayout>& descriptorSetLayout,DerpPipelineInit init)
{
	std::cout << "create pipeline" << std::endl;
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages;

	// vertex + fragment
	vertShader = std::make_unique<DerpShader>(init.vertPath);
	fragShader = std::make_unique<DerpShader>(init.fragPath);
	vertShaderModule = vertShader->createModule(device->handle);
	fragShaderModule = fragShader->createModule(device->handle);
	
	shaderStages.push_back(vk::PipelineShaderStageCreateInfo().
		setStage(vk::ShaderStageFlagBits::eVertex).
		setModule(vertShaderModule).
		setPName("main"));

	shaderStages.push_back(vk::PipelineShaderStageCreateInfo().
		setStage(vk::ShaderStageFlagBits::eFragment).
		setModule(fragShaderModule).
		setPName("main"));

	// tessellation
	if (init.tessellationEnabled)
	{
		tescShader = std::make_unique<DerpShader>(init.tescPath);
		teseShader = std::make_unique<DerpShader>(init.tesePath);
		tescShaderModule = tescShader->createModule(device->handle);
		teseShaderModule = teseShader->createModule(device->handle);
		shaderStages.push_back(vk::PipelineShaderStageCreateInfo().
			setStage(vk::ShaderStageFlagBits::eTessellationControl).
			setModule(tescShaderModule).
			setPName("main"));
		shaderStages.push_back(vk::PipelineShaderStageCreateInfo().
			setStage(vk::ShaderStageFlagBits::eTessellationEvaluation).
			setModule(teseShaderModule).
			setPName("main"));

		numShaderStages += 2;
	}

	// geometry
	if (init.geometryEnabled)
	{
		geomShader = std::make_unique<DerpShader>(init.geomPath);
		geomShaderModule = geomShader->createModule(device->handle);
		shaderStages.push_back(vk::PipelineShaderStageCreateInfo().
			setStage(vk::ShaderStageFlagBits::eGeometry).
			setModule(geomShaderModule).
			setPName("main"));

		numShaderStages += 1;
	}

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	vk::PipelineVertexInputStateCreateInfo vertexInputInfo({}, 1, &bindingDescription, static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());

	vk::PipelineInputAssemblyStateCreateInfo inputAssembly = vk::PipelineInputAssemblyStateCreateInfo();

	if (init.tessellationEnabled)
		inputAssembly.setTopology(vk::PrimitiveTopology::ePatchList);
	else
		inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList);

    vk::Viewport viewport(0.0f, 0.0f, (float)swapChain->extent.width, (float)swapChain->extent.height, 0.0f, 1.0f);

    vk::Rect2D scissor({ 0, 0 }, swapChain->extent);

    vk::PipelineViewportStateCreateInfo viewportState({}, 1, &viewport, 1, &scissor);

	// depth stencil
	vk::PipelineDepthStencilStateCreateInfo depthStencil = vk::PipelineDepthStencilStateCreateInfo().
		setDepthTestEnable(VK_TRUE).
		setDepthWriteEnable(VK_TRUE).
		setDepthCompareOp(vk::CompareOp::eLess).
		setDepthBoundsTestEnable(VK_FALSE).
		setStencilTestEnable(VK_FALSE);

	// rasterizer
	vk::PipelineRasterizationStateCreateInfo rasterizer = vk::PipelineRasterizationStateCreateInfo().
		setDepthClampEnable(VK_FALSE).
		setRasterizerDiscardEnable(VK_FALSE).
		setCullMode(vk::CullModeFlagBits::eNone).
		setFrontFace(vk::FrontFace::eClockwise).
		setDepthBiasEnable(VK_FALSE).
		setLineWidth(1.0f);

	if (init.wireframe)
		rasterizer.setPolygonMode(vk::PolygonMode::eLine);
	else
		rasterizer.setPolygonMode(vk::PolygonMode::eFill);

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

	vk::PipelineTessellationStateCreateInfo tessCreateInfo = vk::PipelineTessellationStateCreateInfo().
		setPatchControlPoints(4);

	
	// create layout
	vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo();

	if (init.hasPushConstants)
	{
		vk::PushConstantRange pushRange(init.pushStage, 0, init.pushSize);
		pipelineLayoutCreateInfo.setPushConstantRangeCount(1);
		pipelineLayoutCreateInfo.setPPushConstantRanges(&pushRange);
	}

	if (init.hasUniform)
	{
		pipelineLayoutCreateInfo.setSetLayoutCount(1);
		pipelineLayoutCreateInfo.setPSetLayouts(&descriptorSetLayout->handle);
	}

    layout = device->handle.createPipelineLayout(pipelineLayoutCreateInfo);

	// create pipeline
	vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo = vk::GraphicsPipelineCreateInfo().
		setStageCount(numShaderStages).
		setPStages(&shaderStages[0]).
		setPVertexInputState(&vertexInputInfo).
		setPInputAssemblyState(&inputAssembly).
		setPViewportState(&viewportState).
		setPRasterizationState(&rasterizer).
		setPMultisampleState(&multisampling).
		setPDepthStencilState(&depthStencil).
		setPColorBlendState(&colorBlending).
		setLayout(layout).
		setRenderPass(renderPass->handle);

	if (init.tessellationEnabled)
		graphicsPipelineCreateInfo.setPTessellationState(&tessCreateInfo);

    handle = device->handle.createGraphicsPipeline(nullptr, graphicsPipelineCreateInfo);
    

	// cleanup
    device->handle.destroyShaderModule(vertShaderModule);
	vertShader.reset();
    device->handle.destroyShaderModule(fragShaderModule);
	fragShader.reset();

	if (init.tessellationEnabled)
	{
		device->handle.destroyShaderModule(tescShaderModule);
		device->handle.destroyShaderModule(teseShaderModule);
	}

	if (init.geometryEnabled)
		device->handle.destroyShaderModule(geomShaderModule);
}


DerpPipeline::~DerpPipeline()
{
}
