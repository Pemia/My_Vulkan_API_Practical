#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

class Surface
{
public:
	VkSurfaceKHR surface;

	void createSurface(VkInstance instance, GLFWwindow* window);

	void clear(VkInstance instance);
};