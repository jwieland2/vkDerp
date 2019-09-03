#include "DerpSampler.h"



DerpSampler::DerpSampler(std::unique_ptr<DerpDevice>& device)
{
	vk::SamplerCreateInfo samplerCreateInfo = vk::SamplerCreateInfo().
		setMagFilter(vk::Filter::eLinear).
		setMinFilter(vk::Filter::eLinear).
		setAddressModeU(vk::SamplerAddressMode::eRepeat).
		setAddressModeV(vk::SamplerAddressMode::eRepeat).
		setAddressModeW(vk::SamplerAddressMode::eRepeat).
		setAnisotropyEnable(VK_TRUE).
		setMaxAnisotropy(16).
		setBorderColor(vk::BorderColor::eIntOpaqueBlack).
		setUnnormalizedCoordinates(VK_FALSE).
		setCompareEnable(VK_FALSE).
		setCompareOp(vk::CompareOp::eAlways).
		setMipmapMode(vk::SamplerMipmapMode::eLinear).
		setMipLodBias(0.0f).
		setMinLod(0.0f).
		setMaxLod(FLT_MAX);

	handle = device->handle.createSampler(samplerCreateInfo);
}


DerpSampler::~DerpSampler()
{
}
