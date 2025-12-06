#pragma once

#include "Graphics_Pipeline/SwapChain.h"

#include <array>

class SwapChainFrameBuffer
{
public:
	std::vector<VkFramebuffer> swapChainFramebuffers;

	void createSwapChainFrameBuffer(VkDevice logicalDevice, SwapChain swapchain, VkRenderPass renderPass);

	void clear(VkDevice logicalDevice);
};