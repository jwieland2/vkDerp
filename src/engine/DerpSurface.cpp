#include "DerpSurface.h"

#include <stdexcept>


DerpSurface::DerpSurface(std::unique_ptr<DerpInstance>& instance, GLFWwindow* window)
{
    std::cout << "create surface" << std::endl;
    
    if (glfwCreateWindowSurface(instance->handle, window, nullptr, &vksurface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
    handle = vk::UniqueSurfaceKHR(vksurface, instance->handle);
}


DerpSurface::~DerpSurface()
{

}
