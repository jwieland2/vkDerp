#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

const double YAW = 0.0f;
const double PITCH = 0.0f;
const double FOV = 100.0f;

class Camera
{
public:



	// Camera Attributes
	glm::vec3 position_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 worldUp;
	// Euler Angles
	float yaw_;
	float pitch_;
	// Camera options
	float fov;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);

	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera();

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();

	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

	
};