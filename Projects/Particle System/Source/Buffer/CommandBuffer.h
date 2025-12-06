#pragma once

#include "Initialize/CommandPool.h"

#include "SwapChainFrameBuffer.h"
#include "VertexBuffer.h"

#include <vector>

class CommandBuffer
{
public:
	// List of Command Buffer Object
	std::vector<VkCommandBuffer> commandBuffers;

	// Function to create Command Buffer
	void createCommandBuffer(VkDevice logicalDevice, CommandPool commandPool, const int maxFrameInFlight);

	// Function to Record Command Buffer
	void recordCommandBuffer(VkCommandBuffer commandBuffer,
									uint32_t imageIndex, 
							    VkRenderPass renderPass, 
								  VkExtent2D swapChainExtent, 
								  VkPipeline graphicsPipeline, 
					    SwapChainFrameBuffer swapChainFramebuffer, 
								VertexBuffer vertexBuffer, 
					   std::vector<uint16_t> vertexIndices);

	// Clear Function
	void clear(VkDevice logicalDevice, CommandPool commandPool);
};