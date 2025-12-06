#pragma once

#include <set>
#include <stdexcept>

#include "QueueFamily.h"
#include "Graphics_Pipeline/SwapChain.h"

const std::vector<const char*> DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
    VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,
    VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    VK_KHR_MAINTENANCE_5_EXTENSION_NAME
};

class Devices
{
public:
    VkPhysicalDevice physicalDevice;
    VkDevice logicalDevice;

    // Function to get Physical Device Information
    void getPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

    // Function to create Logical Device
    void createLogicalDevice(VkSurfaceKHR surface, QueueFamily& queueFamily);

    // Clear Function
    void clear();
private:

    // Check Function
    bool checkDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
};