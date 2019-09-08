#include "GameObjectVertex.h"

GameObjectVertex::GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices)
	: vertices_(vertices)
{
	//vertexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
}

GameObjectVertex::~GameObjectVertex()
{

}