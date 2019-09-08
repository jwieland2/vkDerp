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
	~GameObjectVertex();

	std::vector<Vertex> vertices_;
};