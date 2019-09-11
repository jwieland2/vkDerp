#include "GameObjectVertex.h"

GameObjectVertex::GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices)
	: numVertices(vertices.size()), alloc(&allocator)
{
	objVertexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
}

GameObjectVertex::GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices, std::vector<uint16_t> indices)
	: numVertices(vertices.size()), numIndices(indices.size()), alloc(&allocator)
{
	objVertexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
	objIndexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, indices, allocator);
}

GameObjectVertex::~GameObjectVertex()
{
	vmaDestroyBuffer(*alloc, objVertexBuffer->buffer, objVertexBuffer->allocation);
	if (numIndices)
		vmaDestroyBuffer(*alloc, objIndexBuffer->buffer, objIndexBuffer->allocation);
}