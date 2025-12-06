#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	GLFWwindow* window;

	// Function to Initialize Window of Application by use GLFW library
	void Initialize(std::string windowName = "Particle System", const int width = 800, const int height = 600);
};