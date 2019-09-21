#include "DerpDescriptorSetLayout.h"



DerpDescriptorSetLayout::DerpDescriptorSetLayout(std::unique_ptr<DerpDevice>& device)
{
	std::cout << "create DescriptorSetLayout" << std::endl;


	// 0
	// UBO
	vk::DescriptorSetLayoutBinding uboLayoutBinding = vk::DescriptorSetLayoutBinding().
		setBinding(0).
		setDescriptorCount(1).
		setDescriptorType(vk::DescriptorType::eUniformBuffer).
		setPImmutableSamplers(nullptr).
		setStageFlags(vk::ShaderStageFlagBits::eVertex);

	// 1
	// texture array
	vk::DescriptorSetLayoutBinding layoutBinding = vk::DescriptorSetLayoutBinding().
		setBinding(1).
		setDescriptorCount(8).
		setStageFlags(vk::ShaderStageFlagBits::eFragment).
		setDescriptorType(vk::DescriptorType::eSampledImage).
		setPImmutableSamplers(0);

	// 2
	// sampler
	vk::DescriptorSetLayoutBinding samplerBinding = vk::DescriptorSetLayoutBinding().
		setBinding(2).
		setDescriptorCount(1).
		setStageFlags(vk::ShaderStageFlagBits::eFragment).
		setDescriptorType(vk::DescriptorType::eSampler).
		setPImmutableSamplers(0);

	std::array<vk::DescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, layoutBinding, samplerBinding };

	vk::DescriptorSetLayoutCreateInfo layoutCreateInfo = vk::DescriptorSetLayoutCreateInfo().
		setBindingCount(static_cast<uint32_t>(bindings.size())).
		setPBindings(bindings.data());

    handle = device->handle.createDescriptorSetLayout(layoutCreateInfo);
}


DerpDescriptorSetLayout::~DerpDescriptorSetLayout()
{
}
