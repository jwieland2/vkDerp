#include "Input.h"

#include "game.h"

Input::Input(DerpRenderer* renderer)
{
	firstMouse = true;
	constrainPitch = true;
	setupCallbacks(renderer->window);

	keys.fill(false);
}

Input::~Input()
{

}

void Input::setupCallbacks(GLFWwindow* window)
{
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			app->input->keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			app->input->keys[key] = GL_FALSE;
	}
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
	app->input->scrollOffset = yoffset;
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	auto app = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));

	if (app->input->firstMouse)
	{
		app->input->lastX = xpos;
		app->input->lastY = ypos;
		app->input->firstMouse = false;
	}

	app->camera->yaw_ += ((xpos - app->input->lastX) * app->input->mouseSensitivity);
	app->camera->pitch_ -= ((app->input->lastY - ypos) * app->input->mouseSensitivity);

	app->input->lastX = xpos;
	app->input->lastY = ypos;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (app->input->constrainPitch)
	{
		if (app->camera->pitch_ > 89.0f)
			app->camera->pitch_ = 89.0f;
		if (app->camera->pitch_ < -89.0f)
			app->camera->pitch_ = -89.0f;
	}

	// mouse wheel
	if (app->camera->fov >= 1.0f && app->camera->fov <= 45.0f)
		app->camera->fov -= app->input->scrollOffset;
	if (app->camera->fov <= 1.0f)
		app->camera->fov = 1.0f;
	if (app->camera->fov >= 45.0f)
		app->camera->fov = 45.0f;
}

void Input::process(Camera* camera, float dt)
{
	float velocity = movementSpeed * dt;
	if (this->keys[GLFW_KEY_W])
	{
		camera->position_ += camera->front_ * velocity;
	}
	if (this->keys[GLFW_KEY_S])
	{
		camera->position_ -= camera->front_ * velocity;
	}
	if (this->keys[GLFW_KEY_A])
	{
		camera->position_ -= camera->right_ * velocity;
	}
	if (this->keys[GLFW_KEY_D])
	{
		camera->position_ += camera->right_ * velocity;
	}
}