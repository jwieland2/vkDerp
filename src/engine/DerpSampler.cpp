#include "DerpSampler.h"



DerpSampler::DerpSampler(std::unique_ptr<DerpDevice>& device)
{
	vk::SamplerCreateInfo samplerCreateInfo = vk::SamplerCreateInfo().
		setMagFilter(vk::Filter::eLinear).
		setMinFilter(vk::Filter::eLinear).
		setAddressModeU(vk::SamplerAddressMode::eMirroredRepeat).
		setAddressModeV(vk::SamplerAddressMode::eMirroredRepeat).
		setAddressModeW(vk::SamplerAddressMode::eMirroredRepeat).
		setAnisotropyEnable(VK_FALSE).
		setMaxAnisotropy(16).
		setBorderColor(vk::BorderColor::eIntOpaqueWhite).
		setUnnormalizedCoordinates(VK_FALSE).
		setCompareEnable(VK_FALSE).
		//setCompareOp(vk::CompareOp::eAlways).
		setCompareOp(vk::CompareOp::eNever).
		setMipmapMode(vk::SamplerMipmapMode::eLinear).
		setMipLodBias(0.0f).
		setMinLod(0.0f).
		setMaxLod(FLT_MAX);

	handle = device->handle.createSampler(samplerCreateInfo);
}


DerpSampler::~DerpSampler()
{
}
