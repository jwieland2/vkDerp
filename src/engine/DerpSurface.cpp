#include "DerpSurface.h"

#include <stdexcept>


DerpSurface::DerpSurface(std::unique_ptr<DerpInstance>& instance, GLFWwindow* window)// : instanceRef(instance->handle)
{
    std::cout << "create surface" << std::endl;
    
    if (glfwCreateWindowSurface(instance->handle, window, nullptr, &vksurface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
	//this->instanceRef = instance->handle;
    handle = vk::UniqueSurfaceKHR(vksurface, instance->handle);
	//vkDestroySurfaceKHR(this->instanceRef, vksurface, nullptr);
}


DerpSurface::~DerpSurface()
{
	//vkDestroySurfaceKHR(this->instanceRef, vksurface, nullptr);
}
