#ifndef VALIDATIONLAYER_H
#define VALIDATIONLAYER_H
#endif

#include <vulkan/vulkan.h>

#include <vector>
#include <iostream>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

// If in debug mode
#ifdef NDEBUG
#define enableValidationLayers false
#else
#define enableValidationLayers true
#endif

class ValidationLayer
{
public:
	// Function for check Device Suitable with validation layer extension
	bool checkDeviceValidationLayerSupport();

	// Function to get validation layer extension
	void getValidationLayerExtension(std::vector<const char*> extensions);

	// Function to create information for Debug Message of Validation Layer Extension
	VkDebugUtilsMessengerCreateInfoEXT createDebugMessengerInfo();

	// Function to create Debug Utilities of Debug Message
	VkResult createDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pDebugMessenger
	);

	// Function to set up Debug Message
	void setupDebugMessenger(VkInstance instance);

	// Debug Callback
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);

	// Clear Function
	void DestroyDebugUtilsMessengerEXT(
		VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator
	);

private:
	// Debug Message Object
	VkDebugUtilsMessengerEXT debugMessenger;
};