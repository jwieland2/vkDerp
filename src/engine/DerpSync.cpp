#include "DerpSync.h"



DerpSync::DerpSync(std::unique_ptr<DerpDevice>& device)
{
	std::cout << "create sync objects" << std::endl;

	imageAvailableSemaphore = device->handle.createSemaphore(vk::SemaphoreCreateInfo());
	renderFinishedSemaphore = device->handle.createSemaphore(vk::SemaphoreCreateInfo());

    inFlightFences.resize(maxFramesInFlight);

    for (size_t i = 0; i < maxFramesInFlight; i++)
    {
        inFlightFences[i] = device->handle.createFence(vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
    }
}


DerpSync::~DerpSync()
{
}
