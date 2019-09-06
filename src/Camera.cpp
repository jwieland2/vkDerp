#include "Camera.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>



// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front_(glm::vec3(0.0f, 0.0f, 0.0f)), fov(FOV)
{
	std::cout << "create camera object with vector input" << std::endl;
	position_ = position;
	worldUp = up;
	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
	//up_ = glm::vec3(0.0f, 0.0f, 1.0f);
	//right_ = glm::normalize(glm::cross(up_, glm::vec3(0.0f)));
}
// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: front_(glm::vec3(0.0f, 0.0f, 0.0f)), fov(FOV)
{
	std::cout << "create camera object with scalar input" << std::endl;
	position_ = glm::vec3(posX, posY, posZ);
	worldUp = glm::vec3(upX, upY, upZ);
	yaw_ = yaw;
	pitch_ = pitch;
	updateCameraVectors();
}

Camera::~Camera()
{

}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position_, position_ + front_, up_);
	//return glm::lookAt(position_, glm::vec3(0.0f), up_);
}

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 newfront;
	newfront.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	newfront.y = sin(glm::radians(pitch_));
	newfront.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(newfront);
	// Also re-calculate the Right and Up vector
	right_ = glm::normalize(glm::cross(front_, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up_ = glm::normalize(glm::cross(right_, front_));
}