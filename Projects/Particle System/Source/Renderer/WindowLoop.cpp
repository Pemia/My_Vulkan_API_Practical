#include "WindowLoop.h"

void WindowLoop::drawLoop(Window window, const std::function<void()>& drawFunction)
{
	while (!glfwWindowShouldClose(window.window)) {
		glfwPollEvents();
		drawFunction();
	}
}