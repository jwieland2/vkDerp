#include "DerpShader.h"



DerpShader::DerpShader(const std::string& filename)
{
    code = ResourceManager::readFile(filename);
}


DerpShader::~DerpShader()
{
}

vk::ShaderModule DerpShader::createModule(vk::Device device)
{
    vk::ShaderModuleCreateInfo createInfo({}, code.size(), reinterpret_cast<const uint32_t*>(code.data()));
    return device.createShaderModule(createInfo);
}
