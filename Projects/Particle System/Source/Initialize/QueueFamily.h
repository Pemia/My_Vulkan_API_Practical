#pragma once

#include <vulkan/vulkan.h>

#include <vector>

class QueueFamily
{
public:
    // Queue Family Indices
    int graphicsFamily = -1;
    int presentationFamily = -1;

    // Queue
    VkQueue graphicsQueue;
    VkQueue presentationQueue;

    // Check Function
    bool isValid();

    // Getter Function
    static QueueFamily getQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};