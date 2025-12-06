#include "CommandPool.h"

void CommandPool::createCommandPool(VkDevice logicalDevice, int graphicsQueueFamilyIndex)
{
	// Command Pool Creation Only take two Parameters
	VkCommandPoolCreateInfo poolInfo{};

	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = graphicsQueueFamilyIndex;

	VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to Create Command Pool!");
	}
}

void CommandPool::clear(VkDevice logicalDevice)
{
	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
}
