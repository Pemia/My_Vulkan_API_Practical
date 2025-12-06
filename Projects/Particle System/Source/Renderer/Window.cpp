#include "Window.h"

void Window::Initialize(std::string windowName, const int width, const int height)
{
	// Initialize GLFW
	glfwInit();

	// Set GLFW to work with Vulkan
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}