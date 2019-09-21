#include "DerpDescriptorSet.h"



DerpDescriptorSet::DerpDescriptorSet(
	DerpDescriptorInit& init,
	std::unique_ptr<DerpDevice>& device,
	std::unique_ptr<DerpSwapChain>& swapChain,
	std::unique_ptr<DerpDescriptorSetLayout>& layout,
	std::unique_ptr<DerpDescriptorPool>& descriptorPool,
	std::unique_ptr<DerpBufferUniform>& derpBufferUniform)
{
	std::cout << "create descriptorSet" << std::endl;

	std::array<vk::DescriptorSetLayout, 1> newlayout = { layout->handle };

	vk::DescriptorSetAllocateInfo allocInfo = vk::DescriptorSetAllocateInfo().
		setDescriptorPool(descriptorPool->handle).
		setDescriptorSetCount(1).
		setPSetLayouts(&layout->handle);

	// Allocates an empty descriptor set without actual descriptors from the pool using the set layout
	handle = device->handle.allocateDescriptorSets(allocInfo)[0];

	std::vector<vk::WriteDescriptorSet> writeDescriptorSets;

	// ubo
	uniformDescriptor = vk::DescriptorBufferInfo()
		.setBuffer(derpBufferUniform->uniformBuffers[0])
		.setOffset(0)
		.setRange(sizeof(mvp4));

	writeDescriptorSets.push_back(vk::WriteDescriptorSet()
		.setDstSet(handle)
		.setDstBinding(0)
		.setDstArrayElement(0)
		.setDescriptorCount(1)
		.setDescriptorType(vk::DescriptorType::eUniformBuffer)
		.setPBufferInfo(&uniformDescriptor)
	);


	// texture array
	vk::DescriptorImageInfo descriptorImageInfos[8];

	for (uint32_t i = 0; i < 8; i++)
	{
		descriptorImageInfos[i].sampler = nullptr;
		descriptorImageInfos[i].imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
		descriptorImageInfos[i].imageView = *init.textureView;
	}

	writeDescriptorSets.push_back(vk::WriteDescriptorSet()
		.setDstSet(handle)
		.setDstBinding(1)
		.setDstArrayElement(0)
		.setDescriptorCount(8)
		.setDescriptorType(vk::DescriptorType::eSampledImage)
		.setPImageInfo(descriptorImageInfos)
	);


	// sampler
	samplerDescriptor = vk::DescriptorImageInfo()
		.setSampler(*init.sampler);

	writeDescriptorSets.push_back(vk::WriteDescriptorSet()
		.setDstSet(handle)
		.setDstBinding(2)
		.setDstArrayElement(0)
		.setDescriptorCount(1)
		.setDescriptorType(vk::DescriptorType::eSampler)
		.setPImageInfo(&samplerDescriptor)
	);

	// update
	device->handle.updateDescriptorSets(writeDescriptorSets, nullptr);
}


DerpDescriptorSet::~DerpDescriptorSet()
{
}
