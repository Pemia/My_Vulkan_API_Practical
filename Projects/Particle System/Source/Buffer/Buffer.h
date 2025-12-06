#pragma once

#include <vulkan/vulkan.h>

#include <stdexcept>

class Buffer
{
public:
	// Function to create Buffer
	static void createBuffer(VkPhysicalDevice physicalDevice, 
									 VkDevice logicalDevice,
								 VkDeviceSize size,
						   VkBufferUsageFlags usage,
				VkBufferUsageFlags2CreateInfo usage2,
						VkMemoryPropertyFlags properties,
									VkBuffer& buffer,
							  VkDeviceMemory& bufferMemory);

	// Function to copy Buffer
	static void copyBuffer(VkDevice logicalDevice, 
					  VkCommandPool commandPool, 
						   VkBuffer srcBuffer, 
						   VkBuffer dstBuffer, 
					   VkDeviceSize size,
							VkQueue graphicsQueue);
private:
	static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
};