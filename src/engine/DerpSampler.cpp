#include "DerpSampler.h"



DerpSampler::DerpSampler(DerpSamplerInit& init, std::unique_ptr<DerpDevice>& device)
{
	vk::SamplerCreateInfo samplerCreateInfo = vk::SamplerCreateInfo().
		setMagFilter(vk::Filter::eNearest).
		setMinFilter(vk::Filter::eNearest).
		setAddressModeU(init.repeat).
		setAddressModeV(init.repeat).
		setAddressModeW(init.repeat).
		setAnisotropyEnable(VK_TRUE).
		setMaxAnisotropy(16).
		setBorderColor(vk::BorderColor::eIntOpaqueWhite).
		setUnnormalizedCoordinates(VK_FALSE).
		setCompareEnable(init.compareEnable).
		setCompareOp(init.compareOp).
		setMipmapMode(vk::SamplerMipmapMode::eLinear).
		setMipLodBias(0.0f).
		setMinLod(0.0f).
		setMaxLod(FLT_MAX);

	handle = device->handle.createSampler(samplerCreateInfo);
}


DerpSampler::~DerpSampler()
{
}
