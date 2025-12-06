#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

class RenderPass
{
public:
	VkRenderPass renderPass;

	void createRenderPass(VkDevice logicalDevice, VkFormat swapChainFormat);

	void clear(VkDevice logicalDevice);
};