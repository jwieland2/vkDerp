#include "DerpInstance.h"



DerpInstance::DerpInstance()
{
    std::cout << "create instance" << std::endl;
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    appInfo = { "CIV", VK_MAKE_VERSION(1, 0, 0), "blub", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1 };

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    createInfo = { {}, &appInfo, 0, nullptr, static_cast<uint32_t>(extensions.size()), extensions.data() };

    if (enableValidationLayers)
    {
        debugCreateInfo.flags = {};
        debugCreateInfo.messageSeverity = (vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        debugCreateInfo.messageType = (vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
        debugCreateInfo.pfnUserCallback = debugCallback;

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
        createInfo.pNext = (vk::DebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }

    handle = vk::createInstance(createInfo);
    dynamicLoader.init(handle);

    if (enableValidationLayers)
    {
        debugMessenger = handle.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, dynamicLoader);
    }
}


DerpInstance::~DerpInstance()
{
    //if (enableValidationLayers)
    //    handle.destroyDebugUtilsMessengerEXT(debugMessenger, nullptr, dynamicLoader);

    //handle.destroy();
}

bool DerpInstance::checkValidationLayerSupport()
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const char* layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}

VKAPI_ATTR vk::Bool32 VKAPI_CALL DerpInstance::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    std::cerr << "\tvalidation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

void DerpInstance::printInfo()
{
	std::cout << VK_VERSION_MAJOR(appInfo.apiVersion) << "." << VK_VERSION_MINOR(appInfo.apiVersion) << "." << VK_VERSION_PATCH(appInfo.apiVersion) << std::endl;
	std::cout << appInfo.pApplicationName << std::endl;
	std::cout << appInfo.pEngineName << std::endl;
}