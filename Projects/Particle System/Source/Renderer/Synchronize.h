#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include <stdexcept>

class Synchronize
{
public:
	// Synchronize Object
	std::vector<VkSemaphore> imageAvailableSemaphore;
	std::vector<VkSemaphore> renderFinishedSemaphore;
	std::vector<VkFence> inFlightFence;

	// Function to create Synchronize Object
	void createSyncObject(VkDevice logicalDevice, const int maxFrameInFlight);

	// Function to Clear Synchronize Object
	void clearSynchronizeObject(VkDevice logicalDevice, const int maxFrameInFlight);
};