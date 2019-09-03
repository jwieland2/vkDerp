#include "DerpDescriptorSetLayout.h"



DerpDescriptorSetLayout::DerpDescriptorSetLayout(std::unique_ptr<DerpDevice>& device)
{
	std::cout << "create DescriptorSetLayout" << std::endl;

	vk::DescriptorSetLayoutBinding uboLayoutBinding(
        0,
        vk::DescriptorType::eUniformBuffer,
        1,
        vk::ShaderStageFlagBits::eVertex,
        nullptr
    );

	vk::DescriptorSetLayoutBinding samplerLayoutBinding = vk::DescriptorSetLayoutBinding().
		setBinding(1).
		setDescriptorCount(1).
		setDescriptorType(vk::DescriptorType::eCombinedImageSampler).
		setPImmutableSamplers(nullptr).
		setStageFlags(vk::ShaderStageFlagBits::eFragment);

	std::array<vk::DescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

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
