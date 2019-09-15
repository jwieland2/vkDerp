#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include "engine/vk_mem_alloc.h"

#include "engine/DerpRenderer.h"
#include "engine/DerpBufferLocal.h"


class Terrain
{
public:
	Terrain(DerpRenderer* renderer);
	~Terrain();

	std::unique_ptr<DerpBufferLocal> vertexBuffer;
	std::unique_ptr<DerpBufferLocal> indexBuffer;

	VmaAllocator* alloc;

	const uint32_t PATCH_SIZE = 256;
	const float UV_SCALE = 1.0f;

	struct HeightMap
	{
	private:
		uint8_t *heightdata;
		unsigned char* pixels;
		uint32_t dim;
		uint32_t scale;
		int texWidth, texHeight, texChannels;
	public:
		HeightMap(std::string filename, uint32_t patchsize);
		~HeightMap();
		float getHeight(uint32_t x, uint32_t y);
	};
};