#include "DerpDescriptorSetLayout.h"



DerpDescriptorSetLayout::DerpDescriptorSetLayout(std::unique_ptr<DerpDevice>& device)
{
	std::cout << "create DescriptorSetLayout" << std::endl;

	vk::DescriptorSetLayoutBinding uboLayoutBinding = vk::DescriptorSetLayoutBinding().
		setBinding(0).
		setDescriptorCount(1).
		setDescriptorType(vk::DescriptorType::eUniformBuffer).
		setPImmutableSamplers(nullptr).
		setStageFlags(vk::ShaderStageFlagBits::eTessellationControl | vk::ShaderStageFlagBits::eTessellationEvaluation | vk::ShaderStageFlagBits::eVertex);

	vk::DescriptorSetLayoutBinding heightmapLayoutBinding = vk::DescriptorSetLayoutBinding().
		setBinding(1).
		setDescriptorCount(1).
		setDescriptorType(vk::DescriptorType::eCombinedImageSampler).
		setPImmutableSamplers(nullptr).
		setStageFlags(vk::ShaderStageFlagBits::eTessellationControl | vk::ShaderStageFlagBits::eTessellationEvaluation | vk::ShaderStageFlagBits::eFragment);

	//vk::DescriptorSetLayoutBinding samplerLayoutBinding = vk::DescriptorSetLayoutBinding().
	//	setBinding(2).
	//	setDescriptorCount(1).
	//	setDescriptorType(vk::DescriptorType::eCombinedImageSampler).
	//	setPImmutableSamplers(nullptr).
	//	setStageFlags(vk::ShaderStageFlagBits::eFragment | vk::ShaderStageFlagBits::eTessellationEvaluation | vk::ShaderStageFlagBits::eFragment);

	//std::array<vk::DescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, heightmapLayoutBinding, samplerLayoutBinding };
	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, heightmapLayoutBinding };

    vk::DescriptorSetLayoutCreateInfo layoutInfo(
        {},
        static_cast<uint32_t>(bindings.size()),
        bindings.data()
    );

    handle = device->handle.createDescriptorSetLayout(layoutInfo);
}


DerpDescriptorSetLayout::~DerpDescriptorSetLayout()
{
}
