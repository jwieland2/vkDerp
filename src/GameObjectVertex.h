#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

#include "GameObject.h"
#include "engine/DerpDevice.h"
#include "engine/DerpCommandPool.h"
#include "engine/DerpBufferLocal.h"
#include "engine/DerpRenderer.h"

#include "engine/Vertex.h"

#include "engine/vk_mem_alloc.h"

class GameObjectVertex : public GameObject
{
public:
	GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices);
	GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices, std::vector<uint16_t> indices);
	~GameObjectVertex();

	std::unique_ptr<DerpBufferLocal> objVertexBuffer;
	std::unique_ptr<DerpBufferLocal> objIndexBuffer;
	VmaAllocator* alloc;
	int numVertices = 0, numIndices = 0;
};