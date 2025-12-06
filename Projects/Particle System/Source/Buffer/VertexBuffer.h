#pragma once

#include "Buffer.h"
#include "Graphics_Pipeline/VertexInput.h"

class VertexBuffer
{
public:
	// Vertex Buffer Object
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	// Vertex Index Buffer Object
	VkBuffer vertexIndexBuffer;
	VkDeviceMemory vertexIndexBufferMemory;

	// Function to create Buffer Object
	void createVertexBuffer(VkPhysicalDevice physicalDevice, 
									VkDevice logicalDevice, 
								 VertexInput vertexInput, 
							   VkCommandPool commandPool, 
									 VkQueue graphicsQueue);

	// Function to create Vertex Index Buffer
	void createVertexIndexBuffer(VkPhysicalDevice physicalDevice,
										 VkDevice logicalDevice,
									  VertexInput vertexInput,
									VkCommandPool commandPool,
										  VkQueue graphicsQueue);

	// Clear Function
	void clear(VkDevice logicalDevice);
};