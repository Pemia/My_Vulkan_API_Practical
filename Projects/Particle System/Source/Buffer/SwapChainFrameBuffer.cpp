#include "SwapChainFrameBuffer.h"

void SwapChainFrameBuffer::createSwapChainFrameBuffer(VkDevice logicalDevice, SwapChain swapchain, VkRenderPass renderPass)
{
	// Resize the container to hold all of the FrameBuffers
	swapChainFramebuffers.resize(swapchain.swapChainImages.size());

	// Create a Framebuffer for each Swapchain image
	for (size_t i = 0; i < swapchain.swapChainImages.size(); i++) {
		std::array<VkImageView, 1> attachments = {
			swapchain.swapChainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferCreateInfo{};

		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass;
		framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferCreateInfo.pAttachments = attachments.data();								// List of Attachments (1:1 with Render Pass)
		framebufferCreateInfo.width = swapchain.extent.width;									// Framebuffer width
		framebufferCreateInfo.height = swapchain.extent.height;									// Framebuffer height
		framebufferCreateInfo.layers = 1;														// Framebuffer layers

		VkResult result = vkCreateFramebuffer(logicalDevice, &framebufferCreateInfo, nullptr, &swapChainFramebuffers[i]);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to Create a Framebuffer!");
		}
	}
}

void SwapChainFrameBuffer::clear(VkDevice logicalDevice)
{
	for (auto frameBuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(logicalDevice, frameBuffer, nullptr);
	}
}