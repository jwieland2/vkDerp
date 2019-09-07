#include "GameObjectVertex.h"

GameObjectVertex::GameObjectVertex(std::unique_ptr<DerpDevice>& device, std::unique_ptr<DerpCommandPool>& commandPool, VmaAllocator& allocator, std::vector<Vertex> vertices)
{
	vertexBuffer = std::make_unique<DerpBufferLocal>(device, commandPool, vertices, allocator);
}

GameObjectVertex::~GameObjectVertex()
{

}

void GameObjectVertex::draw(DerpRenderer &renderer)
{
	//model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	renderer.addToCommandBuffer(vertexBuffer.get());
}