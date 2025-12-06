#include "CommandBuffer.h"

void CommandBuffer::createCommandBuffer(VkDevice logicalDevice, CommandPool commandPool, const int maxFrameInFlight)
{
	// Resize command buffer to max frame in flight value
	commandBuffers.resize(maxFrameInFlight);

	// Command Buffer are allocated with the vkAllocateCommandBuffers function, so just take that info
	VkCommandBufferAllocateInfo allocInfo{};

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool.commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data());

	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate Command Buffers!");
	}
}

void CommandBuffer::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkRenderPass renderPass, VkExtent2D swapChainExtent, VkPipeline graphicsPipeline, SwapChainFrameBuffer swapChainFramebuffer, VertexBuffer vertexBuffer, std::vector<uint16_t> vertexIndices)
{	
	// Begin Recording a command buffer
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkResult beginResult = vkBeginCommandBuffer(commandBuffer, &beginInfo);

	if (beginResult != VK_SUCCESS) {
		throw std::runtime_error("Failed to begin Recording Command Buffer!");
	}

	// Drawing start by begining the Render Pass
	VkRenderPassBeginInfo renderPassInfo{};

	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = swapChainFramebuffer.swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	// Clear Color
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	// Render Pass now can begin, it will recordcommands
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	// Now bind Graphics Pipeline
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	// We need to set viewport and scissor state in the command buffer before issuing our draw command because we set them to dynamic in Fixed Function
	VkViewport viewport{};

	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};

	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;

	// Binding Vertex Buffer
	VkBuffer vertexBuffers[] = { vertexBuffer.vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, vertexBuffer.vertexIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	// Time for draw command for Shader
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(vertexIndices.size()), 1, 0, 0, 0);

	// The render pass can now be ended;
	vkCmdEndRenderPass(commandBuffer);

	// now recording the command buffer is finished
	VkResult endResult = vkEndCommandBuffer(commandBuffer);

	if (endResult != VK_SUCCESS) {
		throw std::runtime_error("Failed to Record Command Buffer!");
	}
}

void CommandBuffer::clear(VkDevice logicalDevice, CommandPool commandPool)
{
	for (auto commandBuffer : commandBuffers) {
		vkFreeCommandBuffers(logicalDevice, commandPool.commandPool, 1, &commandBuffer);
	}
}