#include "GameObjectVertex.h"

GameObjectVertex::GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices)
	: vertices_(vertices), num(vertices.size()), alloc(&allocator)
{
	objVertexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
}

GameObjectVertex::~GameObjectVertex()
{
	vmaDestroyBuffer(*alloc, objVertexBuffer->buffer, objVertexBuffer->allocation);
}