#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "../ResourceManager.h"

class DerpShader
{
public:
    DerpShader(const std::string& filename);
    ~DerpShader();

    vk::ShaderModule createModule(vk::Device device);

    std::vector<char> code;
};

