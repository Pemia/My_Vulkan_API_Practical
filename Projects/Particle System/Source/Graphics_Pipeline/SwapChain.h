#pragma once

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>

#include "Initialize/QueueFamily.h"

class SwapChain
{
public:
	// Details of SwapChain(list of swapchain details that device can use)
	VkSurfaceCapabilitiesKHR surfaceCapabilitiesDetails;			// Surface(Application Surface) Properties such as image, size/extent
	std::vector<VkSurfaceFormatKHR> formatDetails;					// Surface image formats such as RGBA, size of each colour
	std::vector<VkPresentModeKHR> presentationModeDetails;			// How images should be presented to screen

	// Objects actually use in later refence
	VkSwapchainKHR swapchain{};
	VkSurfaceFormatKHR surfaceFormat{};
	VkExtent2D extent{};

	// Images and ImageViews of SwapChain
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;

	// Main Function of SwapChain class
	void createSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow* window);

	// Function to get Details of SwapChain
	void getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	// Clear Function
	void clear(VkDevice logicalDevice);
private:
	// Object in class
	VkPresentModeKHR presentMode;

	// Choose Function
	void chooseBestSurfaceFormat();
	void chooseBestPresentationMode();
	void chooseSwapChainExtent(GLFWwindow* window);

	// Function about SwapChain Image and Image View
	std::vector<VkImage> getSwapChainImage(VkDevice logicalDevice, VkSwapchainKHR swapchain);
	VkImageView createSwapChainImageView(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
};