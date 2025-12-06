#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

class CommandPool
{
public:
	VkCommandPool commandPool;

	void createCommandPool(VkDevice logicalDevice, int graphicsQueueFamilyIndex);

	void clear(VkDevice logicalDevice);
};