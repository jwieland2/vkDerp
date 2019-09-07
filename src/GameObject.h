#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();

	glm::mat4 model = glm::mat4(1.0f);

	struct Rotation
	{
		float yaw = 0.0f;
		float pitch = 0.0f;
		float roll = 0.0f;
		//glm::vec3 axis = glm::vec3(0.0f);
	} rotation;

};