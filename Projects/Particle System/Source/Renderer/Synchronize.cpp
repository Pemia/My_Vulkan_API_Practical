#include "Synchronize.h"

void Synchronize::createSyncObject(VkDevice logicalDevice, const int maxFrameInFlight)
{
	// Resize all Synchronize Object to max frame in flight value
	imageAvailableSemaphore.resize(maxFrameInFlight);
	renderFinishedSemaphore.resize(maxFrameInFlight);
	inFlightFence.resize(maxFrameInFlight, VK_NULL_HANDLE);

	// Info of Semaphore
	VkSemaphoreCreateInfo semaphoreInfo{};

	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Info of Fence
	VkFenceCreateInfo fenceInfo{};

	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < maxFrameInFlight; i++) {
		VkResult semaphoreAResult = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]);
		VkResult semaphoreFResult = vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]);
		VkResult fenceResult = vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFence[i]);

		if (semaphoreAResult != VK_SUCCESS && semaphoreFResult != VK_SUCCESS && fenceResult != VK_SUCCESS) {
			throw std::runtime_error("Failed to Create Synchronize Objected!");
		}
	}
}

void Synchronize::clearSynchronizeObject(VkDevice logicalDevice, const int maxFrameInFlight)
{
	for (size_t i = 0; i < maxFrameInFlight; i++) {
		vkDestroySemaphore(logicalDevice, imageAvailableSemaphore[i], nullptr);
		vkDestroySemaphore(logicalDevice, renderFinishedSemaphore[i], nullptr);
		vkDestroyFence(logicalDevice, inFlightFence[i], nullptr);
	}
}