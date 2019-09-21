#include "DerpDescriptorSet.h"



DerpDescriptorSet::DerpDescriptorSet(
	std::unique_ptr<DerpDevice>& device,
	std::unique_ptr<DerpSwapChain>& swapChain,
	std::unique_ptr<DerpDescriptorSetLayout>& layout,
	std::unique_ptr<DerpDescriptorPool>& descriptorPool,
	std::unique_ptr<DerpBufferUniform>& derpBufferUniform,
	std::unique_ptr<DerpImage>& texture,
	//std::unique_ptr<DerpImage>& height,
	std::unique_ptr<DerpSampler>& sampler)
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

		uniformDescriptor = vk::DescriptorBufferInfo()
			.setBuffer(derpBufferUniform->uniformBuffers[0])
			.setOffset(0)
			.setRange(sizeof(UBO));

		//heightDescriptor = vk::DescriptorImageInfo()
		//	.setSampler(sampler->handle)
		//	.setImageView(height->view)
		//	.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);

		imageDescriptor = vk::DescriptorImageInfo()
			.setSampler(sampler->handle)
			.setImageView(texture->view)
			.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
		
		// ubo
		writeDescriptorSets.push_back(vk::WriteDescriptorSet()
			.setDstSet(handle)
			.setDstBinding(0)
			.setDstArrayElement(0)
			.setDescriptorCount(1)
			.setDescriptorType(vk::DescriptorType::eUniformBuffer)
			.setPBufferInfo(&uniformDescriptor));

		//// heightmap
		//writeDescriptorSets.push_back(vk::WriteDescriptorSet()
		//	.setDstSet(handle)
		//	.setDstBinding(1)
		//	.setDstArrayElement(0)
		//	.setDescriptorCount(1)
		//	.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
		//	.setPImageInfo(&heightDescriptor));

		// texture
		writeDescriptorSets.push_back(vk::WriteDescriptorSet()
			.setDstSet(handle)
			.setDstBinding(1)
			.setDstArrayElement(0)
			.setDescriptorCount(1)
			.setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
			.setPImageInfo(&imageDescriptor));

		device->handle.updateDescriptorSets(writeDescriptorSets, nullptr);
}


DerpDescriptorSet::~DerpDescriptorSet()
{
}
