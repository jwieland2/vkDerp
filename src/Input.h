#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "engine/DerpRenderer.h"

class Input
{
public:

	Input(DerpRenderer* renderer);
	~Input();

	std::array<bool, 1024> keys;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void setupCallbacks(GLFWwindow* window);
	void process(Camera* camera, float dt);

	
	double lastX, lastY, mouseSensitivity = 0.3;
	float movementSpeed = 0.03f;
	bool firstMouse, constrainPitch;
	float scrollOffset;

};

