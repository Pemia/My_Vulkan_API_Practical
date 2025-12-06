#include "SwapChain.h"

void SwapChain::createSwapChain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow* window)
{
	getSwapChainDetails(physicalDevice, surface);
	chooseBestSurfaceFormat();
	chooseBestPresentationMode();
	chooseSwapChainExtent(window);

	// How many images are in the swap chain? Get 1 more than the minimum to allow triple buffering
	uint32_t imageCount = surfaceCapabilitiesDetails.minImageCount + 1;

	// If ImageCount higher than max, then clamp down to max
	// If 0, then limitless
	if (surfaceCapabilitiesDetails.maxImageCount > 0 && surfaceCapabilitiesDetails.maxImageCount < imageCount) {
		std::cout << "it's max image in surface! I'm Clamp down surface imageCount" << std::endl;
		imageCount = surfaceCapabilitiesDetails.maxImageCount;
	}

	// Create information for swap chain
	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};
	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = surface;												// Swapchain surface
	swapChainCreateInfo.imageFormat = surfaceFormat.format;								// Swapchain format
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;						// Swapchain colour space
	swapChainCreateInfo.presentMode = presentMode;										// Swapchain presentation mode
	swapChainCreateInfo.imageExtent = extent;											// Swapchain image extent
	swapChainCreateInfo.minImageCount = imageCount;										// Minimum image in Swapchain
	swapChainCreateInfo.imageArrayLayers = 1;											// Number of layer for each image in Swapchain
	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;				// What attachment image will be used as
	swapChainCreateInfo.preTransform = surfaceCapabilitiesDetails.currentTransform;		// Transform to perform on Swapchain images
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;				// How to handle blending images with external graphics (e.g. outher window)
	swapChainCreateInfo.clipped = VK_TRUE;												// Whether to clip part of image not in view (e.g. behind another window, off screen, etc)

	// Get Queue Family Indices
	QueueFamily indices = QueueFamily::getQueueFamilies(physicalDevice, surface);

	// If Graphics and Presentation families are different, then swapchain must let images be shared between familier
	if (indices.graphicsFamily != indices.presentationFamily) {
		uint32_t queuefamilyIndices[] = {
			(uint32_t)indices.graphicsFamily,
			(uint32_t)indices.presentationFamily
		};

		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;		// Image Share handling
		swapChainCreateInfo.queueFamilyIndexCount = 2;							// Number of queue to share images between
		swapChainCreateInfo.pQueueFamilyIndices = queuefamilyIndices;			// Array of queue to sharte between
	}
	else {
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainCreateInfo.queueFamilyIndexCount = 0;
		swapChainCreateInfo.pQueueFamilyIndices = nullptr;
	}

	// Create Swapchain
	VkResult result = vkCreateSwapchainKHR(logicalDevice, &swapChainCreateInfo, nullptr, &swapchain);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create a Swapchain!");
	}

	// Get Images of SwapChain
	swapChainImages = getSwapChainImage(logicalDevice, swapchain);

	// Create ImageViews of SwapChain
	for (VkImage image : swapChainImages) {
		VkImageView imageView_of_swapChain_images = createSwapChainImageView(logicalDevice, image, surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);

		// Add to Swapchain ImageView list
		swapChainImageViews.push_back(imageView_of_swapChain_images);
	}
}

// Get all SwapChain Details that we can use in this Device
void SwapChain::getSwapChainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{

	// *** Capabilities ***
	// Get the surface capabilities for the given surface on the given physical device
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilitiesDetails);

	// *** Formats ***
	uint32_t formatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	// If Formats returned, get list of formats
	if (formatCount != 0) {
		formatDetails.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formatDetails.data());
	}

	// *** Presentation Modes ***
	uint32_t presentationCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, nullptr);

	// If Presentation Modes returned, get list of presentation modes
	if (presentationCount != 0) {
		presentationModeDetails.resize(presentationCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentationCount, presentationModeDetails.data());
	}
}

void SwapChain::clear(VkDevice logicalDevice)
{
	// When Image View been destroy, the Image is destroy
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(logicalDevice, imageView, nullptr);
	}

	// Clear SwapChain
	vkDestroySwapchainKHR(logicalDevice, swapchain, nullptr);
}

// Best Format is subjective, but for this projects it's will be
// Format			:	 VK_FORMAT_R8G8B8A8_UNORM
// ColourSpace		:	 VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
void SwapChain::chooseBestSurfaceFormat()
{
	// If only 1 format available and is undefined, then this mean ALL formats are available (no restrictions)
	if (formatDetails.size() == 1 && formatDetails[0].format == VK_FORMAT_UNDEFINED) {
		surfaceFormat = {VK_FORMAT_R8G8B8A8_UNORM , VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
	}
	// If restricted, search for optimal format
	for (const auto& format : formatDetails) {
		if ((format.format == VK_FORMAT_R8G8B8A8_UNORM || format.format == VK_FORMAT_B8G8R8A8_UNORM) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = format;
			break;
		}
	}
	// If can't find optimal format, then just use first format ("formatDetails[0]") and hope it works
	surfaceFormat = formatDetails[0];
}

// For simply, best Presentation Mode that i use is "VK_PRESENT_MODE_MAILBOX_KHR"
void SwapChain::chooseBestPresentationMode()
{
	for (const auto& presentationMode : presentationModeDetails) {
		if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		}
	}

	// If can't find, use Fifo as Vulkan Spec
	presentMode = VK_PRESENT_MODE_FIFO_KHR;
}

// Choose Swapchain image resolution
void SwapChain::chooseSwapChainExtent(GLFWwindow* window)
{
	// If current Extent is at numeric limits, then extent can vary, Otherwise, it is the size of the window
	if (surfaceCapabilitiesDetails.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		extent = surfaceCapabilitiesDetails.currentExtent;
	}
	else {
		// If value can vary, need to set manually

		// Get Window size
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// Create new Extent using Window size
		VkExtent2D newExtent = {};
		newExtent.width = static_cast<uint32_t>(width);
		newExtent.height = static_cast<uint32_t>(height);

		// Surface also defines max and min, so make sure within boundaries by claming value
		newExtent.width = std::max(surfaceCapabilitiesDetails.minImageExtent.width, std::min(surfaceCapabilitiesDetails.maxImageExtent.width, newExtent.width));
		newExtent.height = std::max(surfaceCapabilitiesDetails.minImageExtent.height, std::min(surfaceCapabilitiesDetails.maxImageExtent.height, newExtent.height));

		extent = newExtent;
	}
}

std::vector<VkImage> SwapChain::getSwapChainImage(VkDevice logicalDevice, VkSwapchainKHR swapchain)
{
	uint32_t swapChainImageCount;
	vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapChainImageCount, nullptr);
	std::vector<VkImage> images(swapChainImageCount);
	vkGetSwapchainImagesKHR(logicalDevice, swapchain, &swapChainImageCount, images.data());

	return images;
}

VkImageView SwapChain::createSwapChainImageView(VkDevice logicalDevice, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewCreateInfo = {};
	viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewCreateInfo.image = image;											// Image to create view for
	viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;						// Type of image (1D, 2D, 3D, Cube, etc)
	viewCreateInfo.format = format;											// Format of image data
	viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;			// Allows remapping of rgba components to other rgba values
	viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

	// Subresources allow the view to view only a part of an image
	viewCreateInfo.subresourceRange.aspectMask = aspectFlags;				// Which aspect of image to view (e.g. COLOR_BIT for viewing colour)
	viewCreateInfo.subresourceRange.baseMipLevel = 0;						// Start mipmap level to view from
	viewCreateInfo.subresourceRange.levelCount = 1;							// Number of mipmap levels to view
	viewCreateInfo.subresourceRange.baseArrayLayer = 0;						// Start array level to view from
	viewCreateInfo.subresourceRange.layerCount = 1;							// Number of array levels to view

	// Create image view and return it
	VkImageView imageView;
	VkResult result = vkCreateImageView(logicalDevice, &viewCreateInfo, nullptr, &imageView);
	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create an image view!");
	}

	return imageView;
}