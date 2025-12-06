#include "Surface.h"

void Surface::createSurface(VkInstance instance, GLFWwindow* window)
{
	// Create Surface by use GLFW
	VkResult result = glfwCreateWindowSurface(instance, window, nullptr, &surface);

	if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Surface!");
	}
}

void Surface::clear(VkInstance instance)
{
	vkDestroySurfaceKHR(instance, surface, nullptr);
}