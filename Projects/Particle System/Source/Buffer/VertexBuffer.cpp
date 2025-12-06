#include "VertexBuffer.h"

void VertexBuffer::createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VertexInput vertexInput, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(vertexInput.vertices[0]) * vertexInput.vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkBufferUsageFlags2CreateInfo usageFlags2StagingBufferInfo{};

	usageFlags2StagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO;
	usageFlags2StagingBufferInfo.usage = VK_BUFFER_USAGE_2_TRANSFER_SRC_BIT;

	Buffer::createBuffer(
		physicalDevice, 
		logicalDevice, 
		bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		usageFlags2StagingBufferInfo, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
		stagingBuffer, 
		stagingBufferMemory
	);


	// Filling the Vertex Buffer
	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertexInput.vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	VkBufferUsageFlags2CreateInfo usageFlags2VertexBufferInfo{};

	usageFlags2VertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO;
	usageFlags2VertexBufferInfo.usage = VK_BUFFER_USAGE_2_VERTEX_BUFFER_BIT_KHR | VK_BUFFER_USAGE_2_TRANSFER_DST_BIT;

	Buffer::createBuffer(
		physicalDevice, 
		logicalDevice, 
		bufferSize, 
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		usageFlags2VertexBufferInfo, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
		vertexBuffer, 
		vertexBufferMemory);

	Buffer::copyBuffer(logicalDevice, commandPool, stagingBuffer, vertexBuffer, bufferSize, graphicsQueue);

	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void VertexBuffer::createVertexIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VertexInput vertexInput, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(vertexInput.vertexIndices[0]) * vertexInput.vertexIndices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkBufferUsageFlags2CreateInfo usageFlags2StagingBufferInfo{};

	usageFlags2StagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO;
	usageFlags2StagingBufferInfo.usage = VK_BUFFER_USAGE_2_TRANSFER_SRC_BIT;

	Buffer::createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, usageFlags2StagingBufferInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);


	// Filling the Vertex Buffer
	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertexInput.vertexIndices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	VkBufferUsageFlags2CreateInfo usageFlags2IndexBufferInfo{};

	usageFlags2IndexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO;
	usageFlags2IndexBufferInfo.usage = VK_BUFFER_USAGE_2_INDEX_BUFFER_BIT_KHR | VK_BUFFER_USAGE_2_TRANSFER_DST_BIT;

	Buffer::createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, usageFlags2IndexBufferInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexIndexBuffer, vertexIndexBufferMemory);

	Buffer::copyBuffer(logicalDevice, commandPool, stagingBuffer, vertexIndexBuffer, bufferSize, graphicsQueue);

	// Clear unused data
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void VertexBuffer::clear(VkDevice logicalDevice)
{
	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
	vkDestroyBuffer(logicalDevice, vertexIndexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexIndexBufferMemory, nullptr);
}