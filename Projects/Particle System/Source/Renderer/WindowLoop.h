#pragma once

#include <functional>

#include "Window.h"

class WindowLoop
{
public:
	static void drawLoop(Window window, const std::function<void()>& drawFunction);
};